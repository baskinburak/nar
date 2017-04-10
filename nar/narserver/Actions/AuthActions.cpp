#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/narserver/Actions/AuthActions.h>
#include <nar/lib/Messaging/messaging_utility.h>


void nar::AuthAction::authentication_dispatcher(nar::ServerGlobal* s_global, nar::Socket* skt, nar::DBStructs::User& user) {
    std::string message = get_message(skt);
    std::string action = Messaging::get_action(message);
    if(action == std::string("file_push_request")) {
        nar::MessageTypes::FilePush::Request req;
        req.receive_message(message);
        push_file_action(s_global,skt,req,user);
    } else if(action == std::string("file_pull_request")) {

    } else if(action == std::string("machine_register")) {
        nar::MessageTypes::MachineRegister::Request req;
        req.receive_message(message);
        machine_register_action(s_global,skt,req,user);
    }
}

void nar::AuthAction::push_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::FilePush::Request& req, nar::DBStructs::User& user) {
    unsigned long long int file_size = req.get_file_size();
    std::string dir_name = req.get_dir_name();
    std::string file_name = req.get_file_name();

    nar::Database* db = s_global->get_db();

     unsigned long long int peer_num = (file_size / CHUNK_SIZE);
     unsigned long long int remainder = file_size - (peer_num*CHUNK_SIZE);
     if( file_size - (peer_num*CHUNK_SIZE) ) peer_num++;

    unsigned short r_port = s_global->get_randezvous_port();

    nar::MessageTypes::FilePush::Response p_resp(200, r_port);

    nar::SockInfo* peer_sock;

    std::vector<nar::DBStructs::Chunk> v_chunks;
    std::vector<nar::DBStructs::ChunkToMachine> v_chunktomac;
    std::vector<>std::string> v_macid;

    long long int c_id = db->getNextChunkIds(peer_num);
    long long int f_id = db->getNextFileIds(1);
    c_id--;
    for(int i=0;i<peer_num;i++) {
        nar::DBStructs::Chunk chnk;
        nar::DBStructs::ChunkToMachine c_to_m;

        c_id++;
        long long int c_size = CHUNK_SIZE;
        if ( peer_num-1 == i) c_size = remainder;
        long long int s_id = s_global->get_next_stream_id();

        nar::MessageTypes::WaitChunkPush::Request chunk_req(r_port, s_id, c_id, c_size);
        nar::MessageTypes::WaitChunkPush::Response chunk_resp;
        do {
            peer_sock = peerSelect(...);
            chunk_req.send_mess(peer_sock->getSck(), chunk_resp);
        } while(chunk_resp.get_status_code() != 200)

        v_macid.push_back(peer_sock.get_machine_id());
        p_resp.add_element(peer_sock->get_machine_id(), c_id, s_id, c_size);
    }

    p_resp.send_mess(skt);

    // WAIT FOR CONFIRMATION IF THERE ARE ANY

    nar::DBStructs::File fl;
    fl.file_id = f_id;
    fl.file_name = file_name;
    fl.file_size = file_size;
    db->insertFile(fl);

    for(int i=0;i<peer_num;i++){
        nar::DBStructs::Chunk chnk;
        nar::DBStructs::ChunkToMachine c_to_m;
        c_to_m.chunk_id = chnk.chunk_id = c_id;
        chnk.file_id = f_id ;
        chnk.chunk_size = CHUNK_SIZE;
        if (i == peer_num-1) chnk.chunk_size = remainder;
        db->insertChunk(chnk);
        c_to_m.machine_id = v_macid[i];
        db->insertChunkToMachine(c_to_m);
    }
    return;
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
