#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/narserver/Actions/AuthActions.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>
#include <nar/lib/Messaging/MessageTypes/DeleteMachineChunk.h>
#include <nar/lib/Messaging/MessageTypes/InfoChunkPull.h>


void nar::AuthAction::authentication_dispatcher(nar::ServerGlobal* s_global, nar::Socket* skt, nar::DBStructs::User& user) {
    std::string message = get_message(skt);
    std::string action = Messaging::get_action(message);

    if(action == std::string("file_push_request")) {
        nar::MessageTypes::FilePush::Request req;
        req.receive_message(message);
        push_file_action(s_global,skt,req,user);
    } else if(action == std::string("file_pull_request")) {
        nar::MessageTypes::FilePull::Request req;
        try {
            req.receive_message(message);
        }
        catch( ... ) {
            NAR_LOG<<"Malformed Pull request received, ignored."<<std::endl;
            nar::MessageTypes::FilePull::Response resp(300,-1);
            try {
                resp.send_mess(skt);
            } catch ( ... ) { }
            return;
        }
        try {
            pull_file_action(s_global,skt,req,user);
        }
        catch( nar::Exception::MessageTypes::PullFileDoesNotExist& er) {              // Wanted file does not exist
            nar::MessageTypes::FilePull::Response resp(300,-1);         // STATUS CODE?
            try {
                resp.send_mess(skt);
            } catch ( ... ) { }
        }
    } else if(action == std::string("machine_register")) {
        nar::MessageTypes::MachineRegister::Request req;
        req.receive_message(message);
        machine_register_action(s_global,skt,req,user);
    } else if(action == std::string("get_dir_info"))  {
        nar::MessageTypes::DirInfo::Request req;
        try {
            req.receive_message(message);
        } catch(nar::Exception::MessageTypes::BadMessageReceive exp) {
            NAR_LOG<<exp.what()<<std::endl;
            nar::MessageTypes::DirInfo::Response resp(300);
            resp.send_mess(skt);
            return;
        }

        dir_info_action(s_global,skt,req,user);
    } else if(action == std::string("mkdir")) {
        nar::MessageTypes::Mkdir::Request req;
        req.receive_message(message);
        mkdir_action(s_global,skt,req,user);
    } else if(action == std::string("delete_file")) {
        nar::MessageTypes::DeleteFile::Request req;
        try {
            req.receive_message(message);
        } catch(nar::Exception::MessageTypes::BadMessageReceive exp) {
            NAR_LOG<<exp.what()<<std::endl;
            nar::MessageTypes::DeleteFile::Response resp(300);
            resp.send_mess(skt);
            return;
        }

        delete_file_action(s_global,skt,req,user);
    }
}


long long int findFileId(std::string& file_name,std::string& dir_name,std::string& uname, nar::Database *db)
{// returns -1 in any case of problem
    long long int file_id = -1;
    nar::DBStructs::Directory dir;

    dir  = db->findDirectoryId(uname,dir_name);
    if(dir.dir_id == -1)
    {
        NAR_LOG<<"No such User directory pair"<<std::endl;
        throw nar::Exception::MessageTypes::PullFileDoesNotExist("File does not exist",300);
    } else {
        std::vector<nar::DBStructs::File> files = db->getDirectoryFile(dir.dir_id);
        for(int i= 0;i<files.size();i++){
            if(files[i].file_name.compare(file_name)== 0) {
             file_id = files[i].file_id;
            }
        }
    }
    return file_id;
}
void nar::AuthAction::delete_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::DeleteFile::Request& req, nar::DBStructs::User& user) {
    nar::Database* db = s_global->get_db();
    std::string u_name = user.user_name;
    std::string& d_name = req.get_dest_dir();
    std::string& f_name = req.get_file_name();
    long long int file_id;

    try{
        nar::DatabaseReadLock read_lock(db);
        file_id = findFileId(f_name, d_name, u_name, db);
    } catch(...) {
        nar::MessageTypes::DeleteFile::Response resp(400);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }

    if(file_id == -1) {
        NAR_LOG<<"There is no such file"<<std::endl;
        nar::MessageTypes::DeleteFile::Response resp(300);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }
    //NAR_LOG << "here here" << std::endl;
    std::vector<struct DBStructs::Chunk> chunks;
    try{
        nar::DatabaseReadLock read_lock(db);
        chunks  = db->getChunks(file_id);
        //NAR_LOG << "your chunks, sir" << std::endl;
    } catch(...) {
        NAR_LOG << "Server delete file getchunk error------fileid "<<std::endl;
        nar::MessageTypes::DeleteFile::Response resp(401);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }
    //NAR_LOG << "all chunks" << std::endl;
    for(int i=0;i<chunks.size();i++) {
        std::vector<struct DBStructs::Machine> chunk_machines;

        try {
           // NAR_LOG << "machines" << std::endl;
            nar::DatabaseReadLock read_lock(db);
            chunk_machines= db->getMachines(chunks[i].chunk_id);
           // NAR_LOG << "machines2" << std::endl;

        } catch(...) {
            NAR_LOG<<"Server delete file getmachines error----chunkid "<<chunks[i].chunk_id<<std::endl;
            nar::MessageTypes::DeleteFile::Response resp(402);
            try {
                resp.send_mess(skt);
            }catch(...) {
                NAR_LOG<<"send_mess_bomb"<<std::endl;
                return;
            }
            return;
        }
        //NAR_LOG << "chunk machinessssss ay" << std::endl;
        for(int j=0;j<chunk_machines.size();j++) {
            nar::SockInfo* peer_sck;
            try {
                //NAR_LOG << "efendim" << std::endl;
                peer_sck = s_global->peers->get_peer(chunk_machines[j].machine_id );
                //NAR_LOG << "olur" << std::endl;
            } catch(...) {
                NAR_LOG <<"Server delete file get_peer error----machine_id "<<chunk_machines[j].machine_id<<std::endl;
                nar::MessageTypes::DeleteFile::Response resp(500);
                try {
                    resp.send_mess(skt);
                }catch(...) {
                    NAR_LOG<<"send_mess_bomb"<<std::endl;
                    return;
                }
                return;
            }
            if(peer_sck ==NULL) {
                struct nar::DBStructs::Machine t_mac;
                try {
                    nar::DatabaseReadLock read_lock(db);
                    t_mac =  db->getMachine(chunk_machines[j].machine_id);
                } catch(...) {
                    NAR_LOG<<"Server delete file getMachine error----machine_id "<<chunk_machines[j].machine_id<<std::endl;
                    nar::MessageTypes::DeleteFile::Response resp(403);
                    try {
                        resp.send_mess(skt);
                    }catch(...) {
                        NAR_LOG<<"send_mess_bomb"<<std::endl;
                        return;
                    }
                    return;
                }
                std::string cur_value = t_mac.delete_list;
                if (cur_value.size() > 0) {
                    t_mac.delete_list = cur_value + std::string(",")+ std::to_string(chunks[i].chunk_id);
                } else {
                    t_mac.delete_list = std::to_string(chunks[i].chunk_id);
                }
                try {
                    nar::DatabaseWriteLock write_lock(db);
                    db->updateMachineDeleteList(t_mac);
                } catch (...) {
                    NAR_LOG<<"Server delete file updateMachineDeleteList error----machine_id "<<chunk_machines[j].machine_id<<std::endl;
                    nar::MessageTypes::DeleteFile::Response resp(404);
                    try {
                        resp.send_mess(skt);
                    }catch(...) {
                        NAR_LOG<<"send_mess_bomb"<<std::endl;
                        return;
                    }
                    return;
                }



            } else {
                nar::MessageTypes::DeleteMachineChunk::Request del_req(std::to_string(chunks[i].chunk_id));
                nar::MessageTypes::DeleteMachineChunk::Response del_resp(200);

                try {
                    del_req.send_mess(peer_sck->get_sck(),del_resp);

                } catch (nar::Exception::MessageTypes::InternalDaemonError exp) {
                    NAR_LOG<< "AuthAction::Delete" << exp.what()<<std::endl;
                    struct nar::DBStructs::Machine t_mac;
                    try {
                        nar::DatabaseReadLock read_lock(db);
                        t_mac =  db->getMachine(chunk_machines[j].machine_id);
                    } catch(...) {
                        NAR_LOG<<"Server delete file getMachine error----machine_id "<<chunk_machines[j].machine_id<<std::endl;
                        nar::MessageTypes::DeleteFile::Response resp(405);
                        try {
                            resp.send_mess(skt);
                        }catch(...) {
                            NAR_LOG<<"send_mess_bomb"<<std::endl;
                            return;
                        }
                        return;
                    }
                    std::string cur_value = t_mac.delete_list;
                    if (cur_value.size() > 0) {
                        t_mac.delete_list = cur_value + std::string(",")+ std::to_string(chunks[i].chunk_id);
                    } else {
                        t_mac.delete_list = std::to_string(chunks[i].chunk_id);
                    }

                    try {
                        nar::DatabaseWriteLock write_lock(db);
                        db->updateMachineDeleteList(t_mac);
                    } catch (...) {
                        NAR_LOG<<"Server delete file updateMachineDeleteList error----machine_id "<<chunk_machines[j].machine_id<<std::endl;
                        nar::MessageTypes::DeleteFile::Response resp(406);
                        try {
                            resp.send_mess(skt);
                        }catch(...) {
                            NAR_LOG<<"send_mess_bomb"<<std::endl;
                            return;
                        }
                        return;
                    }
                }


            }






        }

    }
    struct nar::DBStructs::File m_file;
    struct nar::DBStructs::Directory m_dir;
    try {
        nar::DatabaseReadLock read_lock(db);
        m_dir= db->findDirectoryId(u_name ,d_name);
    } catch(sql::SQLException & e) {
        NAR_LOG<<e.what()<<std::endl;
        nar::MessageTypes::DeleteFile::Response resp(318);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }


    struct nar::DBStructs::DirectoryTo dir_to;
    dir_to.dir_id = m_dir.dir_id;
    dir_to.item_id = file_id;
    dir_to.ForD = 0;
    m_file.file_id = file_id;

    try {
        nar::DatabaseWriteLock write_lock(db);
        db->deleteFile(m_file);
    } catch (...) {
        NAR_LOG<<"File could not be deleted from Database"<<std::endl;
        nar::MessageTypes::DeleteFile::Response resp(407);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }
    nar::MessageTypes::DeleteFile::Response resp(200);
    try {
        resp.send_mess(skt);
    }catch(...) {
        NAR_LOG<<"send_mess_bomb"<<std::endl;
        return;
    }
    try {
        nar::DatabaseWriteLock write_lock(db);
        db->deleteDirectoryTo(dir_to);
    } catch(sql::SQLException& e) {
        NAR_LOG<<e.what()<<std::endl;
        nar::MessageTypes::DeleteFile::Response resp(408);
        try {
            resp.send_mess(skt);
        }catch(...) {
            NAR_LOG<<"send_mess_bomb"<<std::endl;
            return;
        }
        return;
    }
    return;
}


void nar::AuthAction::mkdir_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::Mkdir::Request& req, nar::DBStructs::User& user) {
    std::string target_dir = req.get_dest_dir();
    std::string dir_name = req.get_dir_name();
    nar::Database* db = s_global->get_db();
    nar::DBStructs::Directory pwd;
    try{
        nar::DatabaseReadLock read_lock(db);
        pwd = db->findDirectoryId(user.user_name,target_dir);
    } catch(sql::SQLException& e) {
        NAR_LOG<<e.what()<<std::endl;
        nar::MessageTypes::Mkdir::Response resp(401);
        try{
            resp.send_mess(skt);
        } catch(...) {
            NAR_LOG<<"send mess bomb mkdir"<<std::endl;
        }

    }
    if (pwd.dir_id == -1) {
        nar::MessageTypes::Mkdir::Response resp(402);
        try{
            resp.send_mess(skt);
        } catch(...) {
            NAR_LOG<<"send mess bomb mkdir"<<std::endl;
        }
    }
    nar::DBStructs::Directory new_dir;
    new_dir.dir_name = dir_name;

    try {
        nar::DatabaseWriteLock write_lock(db);
        new_dir.dir_id = db->getNextDirectoryId(1);
        db->insertDirectory(new_dir);          // DIR_ID ASSIGNED
    } catch(sql::SQLException& e){
        NAR_LOG<<e.what()<<std::endl;
        nar::MessageTypes::Mkdir::Response resp(403);
        try{
            resp.send_mess(skt);
        } catch(...) {
            NAR_LOG<<"send mess bomb mkdir"<<std::endl;
        }
    }



    nar::DBStructs::DirectoryTo tmp;
    tmp.dir_id=pwd.dir_id;
    tmp.item_id=new_dir.dir_id;
    tmp.ForD = true;

    try {
        nar::DatabaseWriteLock write_lock(db);
        db->insertDirectoryTo(tmp);
    } catch(sql::SQLException& e) {
        NAR_LOG<<e.what()<<std::endl;
        nar::MessageTypes::Mkdir::Response resp(404);
        try{
            resp.send_mess(skt);
        } catch(...) {
            NAR_LOG<<"send mess bomb mkdir"<<std::endl;
        }
    }

    nar::MessageTypes::Mkdir::Response resp(200);
    resp.send_mess(skt);
    return;
}

// @throws: PullFileDoesNotExist
void nar::AuthAction::pull_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::FilePull::Request& req, nar::DBStructs::User&  user) {
    std::string file_name = req.get_file_name();
    std::string dir_name = req.get_dir_name();
    nar::MessageTypes::FilePull::Response resp;

    nar::Database* db = s_global->get_db();
    long long int f_id = -1;
    try {
        nar::DatabaseReadLock read_lock(db);
        f_id = findFileId(file_name,dir_name,user.user_name, db);


    } catch (sql::SQLException &e) {
        NAR_LOG<<e.what()<<std::endl;
        resp.set_status_code(315);
        try {

            resp.send_mess(skt);
        } catch (...) {}
        return;
    } catch(...) {
        NAR_LOG<<"findFileId unkown error"<<std::endl;
        resp.set_status_code(316);
        try {
            resp.send_mess(skt);
        } catch (...) {}
        return;
    }

    if (f_id == -1) {
        resp.set_status_code(315);
        NAR_LOG << "File pull request with non-existing file" << std::endl;
        try {
            resp.send_mess(skt);
        } catch (...) {}
        return;
    }

    std::vector<struct DBStructs::Chunk> chunks;
    try {
        nar::DatabaseReadLock read_lock(db);
        chunks = db->getChunks(f_id);
    } catch(sql::SQLException & e) {
        NAR_LOG<<e.what()<<std::endl;
        resp.set_status_code(318);
        try {
            resp.send_mess(skt);
        } catch (...) {}
        return;
    }

    unsigned short r_port;

    try {
        r_port = s_global->get_randezvous_port();
        resp.set_rendezvous_port(r_port);
    }
    catch ( nar::Exception::USocket::NoAvailablePort& e ) {
        resp.set_status_code(500);                              // STATUS CODE?
    }

    for(int i=0; i < chunks.size(); ++i) {
        std::vector<struct DBStructs::Machine> machines;
        try {
            nar::DatabaseReadLock read_lock(db);
            machines = db->getMachines(chunks[i].chunk_id);
        } catch(sql::SQLException & e) {
            NAR_LOG<<e.what()<<std::endl;
            resp.set_status_code(319);
            try {
                resp.send_mess(skt);
            } catch (...) {}
            return;
        }

        nar::SockInfo* peer_sck;
        int j;
        for( j=0; !(peer_sck = s_global->peers->get_peer(machines[j].machine_id )) && (j < machines.size()) ; ++j);

        if (j == machines.size()) {     // Chunk is not online
            resp.set_status_code(515);
            try {
                resp.send_mess(skt);
            } catch (...) {}
            return;
        }


        long long int s_id = s_global->get_next_stream_id();

        nar::MessageTypes::WaitChunkPull::Request chunk_req(r_port, s_id, chunks[i].chunk_id, chunks[i].chunk_size);
        nar::MessageTypes::WaitChunkPull::Response chunk_resp;
        chunk_req.send_mess(peer_sck->get_sck(), chunk_resp);
        if (chunk_resp.get_status_code() != 200 )
            NAR_LOG << "Pull Server: Peer does not respond to WaitChunkPull" << std::endl;

        resp.add_element(peer_sck->get_machine_id(), chunks[i].chunk_id, s_id, chunks[i].chunk_size, chunks[i].hashed);
    }
    try {
        resp.set_status_code(200);
        resp.send_mess(skt);
    } catch(...) {}
    while(true) {
        std::string message = get_message(skt);
        nar::MessageTypes::InfoChunkPull::Request req;
        try {
            req.receive_message(message);
        } catch(nar::Exception::Daemon::ActiveChunkError & e) {
            NAR_LOG<<e.what()<<std::endl;
            nar::DBStructs::Chunk ck;
            long long int cid= req.get_chunk_id();
            try {
                nar::DatabaseReadLock read_lock(db);
                ck = db->getChunk(cid);
            } catch(sql::SQLException &err) {
                NAR_LOG<<err.what()<<"second try for active pull action"<<std::endl;
                return;
            }
            std::vector<struct DBStructs::Machine> machines = db->getMachines(cid);
            nar::SockInfo* peer_sck;
            for(int j=0; !(peer_sck = s_global->peers->get_peer(machines[j].machine_id )); ++j);


            long long int s_id = s_global->get_next_stream_id();

            nar::MessageTypes::WaitChunkPull::Request chunk_req(r_port, s_id, cid, ck.chunk_size);
            nar::MessageTypes::WaitChunkPull::Response chunk_resp;
            chunk_req.send_mess(peer_sck->get_sck(), chunk_resp);
            if (chunk_resp.get_status_code() != 200 )
                NAR_LOG << "Pull Server: Peer does not respond to WaitChunkPull" << std::endl;
            nar::MessageTypes::InfoChunkPull::Response resp(s_id,200);
            resp.send_mess(skt);
        } catch(...) {
            return;
        }
        if (req.get_success() == 200) {
            break;
        }

    }
    return;
}

void nar::AuthAction::dir_info_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::DirInfo::Request& req, nar::DBStructs::User& user) {
    nar::Database* db = s_global->get_db();
    std::string& dir_name = req.get_dir();
    nar::MessageTypes::DirInfo::Response resp(200);
    struct DBStructs::Directory dir;
    std::vector<nar::DBStructs::File> files;
    std::vector<nar::DBStructs::Directory> dirs;
    try {
        nar::DatabaseReadLock read_lock(db);
        dir = db->findDirectoryId(user.user_name, dir_name);
    } catch (...) {
        NAR_LOG<<"Server dir info action findDirectoryId error"<<std::endl;
        resp.set_status_code(400);
        resp.send_mess(skt);
        return;
    }
    try {
        nar::DatabaseReadLock read_lock(db);
        files = db->getDirectoryFile(dir.dir_id);
    } catch (...) {
        NAR_LOG<<"Server dir info action getDirectoryFile error"<<std::endl;
        resp.set_status_code(401);
        resp.send_mess(skt);
        return;
    }
    try {
        nar::DatabaseReadLock read_lock(db);
        dirs = db->getDirectoryDir(dir.dir_id);
    } catch (...) {
        NAR_LOG<<"Server dir info action getDirectoryDir error"<<std::endl;
        resp.set_status_code(402);
        resp.send_mess(skt);
        return;
    }



    for (size_t i = 0; i < dirs.size(); i++) {
        struct DBStructs::Directory& tmpDir = dirs[i];
        resp.add_element(std::to_string(tmpDir.change_time),tmpDir.dir_id,tmpDir.dir_name, tmpDir.dir_size, true );
    }

    for (size_t i = 0; i < files.size(); i++) {
        struct DBStructs::File& tmpFile = files[i];
        resp.add_element(std::to_string(tmpFile.change_time),tmpFile.file_id,tmpFile.file_name, tmpFile.file_size, false );
    }
    NAR_LOG << "Dir Info response is being sent." << std::endl;
    resp.send_mess(skt);
    return;
}




void nar::AuthAction::push_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::FilePush::Request& req, nar::DBStructs::User& user) {
    unsigned long long int file_size = req.get_file_size();
    if(file_size < 0) {
        return;
    }
    std::string dir_name = req.get_dir_name();
    std::string file_name = req.get_file_name();

    nar::Database* db = s_global->get_db();

    unsigned long long int peer_num = ((file_size + CHUNK_SIZE - 1) / CHUNK_SIZE);
    unsigned short r_port = s_global->get_randezvous_port();

    nar::MessageTypes::FilePush::Response p_resp(200, r_port);


    nar::SockInfo* peer_sock;

    std::vector<nar::DBStructs::Chunk> v_chunks;
    std::vector<nar::DBStructs::ChunkToMachine> v_chunktomac;
    std::map<long long int, std::vector<std::string> > v_macid;
    long long int c_id;
    long long int f_id;
    {
        nar::DatabaseReadLock some_test_lock(db);
        c_id = db->getNextChunkId(peer_num);
        f_id = db->getNextFileId(1);
    }


    long long int f_cid = c_id;
    unsigned long long int f_size = file_size;

    for(int i=0; i<peer_num; i++, c_id++, f_size -= CHUNK_SIZE) {
        long long int c_size = std::min(CHUNK_SIZE, f_size);

        std::vector<nar::SockInfo*>* peer_socks;

        try {
            peer_socks = s_global->peers->peer_select(user, CHUNK_SIZE, 2);
        } catch(nar::Exception::Peers::NoValidPeer& exp) {
            p_resp.set_status_code(707);
            p_resp.send_mess(skt);
            return;
        }

        /*try {
            peer_sock = s_global->peers->peer_select(user, CHUNK_SIZE);
        } catch(nar::Exception::Peers::NoValidPeer& exp) {
            p_resp.set_status_code(707);
            p_resp.send_mess(skt);
            return;
        }*/

        for(int i=0; i<peer_socks->size(); i++) {
            long long int s_id = s_global->get_next_stream_id();
            nar::MessageTypes::WaitChunkPush::Request chunk_req(r_port, s_id, c_id, c_size);
            nar::MessageTypes::WaitChunkPush::Response chunk_resp;
            peer_sock = (*peer_socks)[i];
            chunk_req.send_mess(peer_sock->get_sck(), chunk_resp);
            v_macid[c_id].push_back(peer_sock->get_machine_id());
            p_resp.add_element(std::string(""), c_id, s_id, c_size);
        }

    }

    p_resp.send_mess(skt);

    // WAIT FOR CONFIRMATION IF THERE ARE ANY

    try {
        nar::DBStructs::File fl;
        fl.file_id = f_id;
        fl.file_name = file_name;
        fl.file_size = file_size;
        nar::DBStructs::DirectoryTo dt;
        dt.dir_id = user.dir_id;
        dt.item_id = f_id;
        dt.ForD = false;
        {
            nar::DatabaseWriteLock write_lock(db);
            db->insertFile(fl);
            db->insertDirectoryTo(dt);
        }
        f_size = file_size;
        for(int i=0;i<peer_num;i++, f_size -= CHUNK_SIZE){
            nar::DBStructs::Chunk chnk;
            nar::DBStructs::ChunkToMachine c_to_m;
            c_to_m.chunk_id = chnk.chunk_id = f_cid++;
            chnk.file_id = f_id ;
            chnk.chunk_size = std::min(CHUNK_SIZE, f_size);
            {
                nar::DatabaseWriteLock write_lock(db);
                db->insertChunk(chnk);
                for(int j=0; j<v_macid[c_to_m.chunk_id].size(); j++) {
                    c_to_m.machine_id = v_macid[c_to_m.chunk_id][j];
                    db->insertChunkToMachine(c_to_m);
                }
            }
        }
    } catch(...) {
        NAR_LOG<<"server_push_bad try"<<std::endl;
        //rollback TODO !!!!
    }
}

void nar::AuthAction::machine_register_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::MachineRegister::Request& req, nar::DBStructs::User& user) {

    long long int machine_quota = req.get_machine_quota();
    long long int machine_diskspace = req.get_machine_diskspace();

    nar::DBStructs::Machine mac;
    mac.machine_quota = machine_quota;
    mac.machine_diskSpace = machine_diskspace;
    mac.user_id = user.user_id;
    nar::Database* db = s_global->get_db();

    std::string mac_id;
    while (1) {
        AesCryptor::generate_key(mac_id,150);
        try {
            mac.machine_id = mac_id;
            nar::DatabaseWriteLock write_lock(db);
            db->insertMachine(mac);
            break;
        } catch (...) {
            continue;
        }
    }
    nar::MessageTypes::MachineRegister::Response resp(200,mac_id);
    resp.send_mess(skt);
    return;
}
