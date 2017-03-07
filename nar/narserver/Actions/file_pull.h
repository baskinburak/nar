#ifndef NAR_ACTION_FILE_PULL_H
#define NAR_ACTION_FILE_PULL_H

#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/sockinfo.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/narserver/dbstructs.h>
#include <iostream>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::string;

namespace nar {
    namespace Actions {

        long long int findFileId(string& file_name,string& dir_name,string& uname, nar::Database* db){// returns -1 in any case of problem
            long long int file_id = -1;
            nar::Directory dir = db->findDirectoryId(uname,dir_name);
            if(dir.dir_id == -1){
                cout<<"No such User directory pair"<<endl;
            } else{
                std::vector<nar::File> files = db->getDirectoryFile(dir.dir_id);
                for(int i= 0;i<files.size();i++){
                    if(files[i].file_name.compare(file_name)== 0){
                        file_id = files[i].file_id;
                    }
                }
            }
            return file_id;

        }


        std::vector<long long int> findPosUsers(long long int  f_id, nar::Database* db, nar::ServerGlobal* s_global) {
            std::vector<nar::User> users = db->getUserFromFile(f_id);
            std::vector<long long int> output;
            for(int i= 0;i<users.size();i++){
                auto it = s_global->keepalives.find(users[i].user_name);
                if(it!= s_global->keepalives.end()){
                    output.push_back(users[i].user_id);
                }
            }
            return output;
        }


        void sendMachineListPull(nar::SockInfo* inf, int status, std::vector< std::pair<long long int, std::vector<nar::Machine> > > & cTom, std::vector< std::pair<long long int,std::vector<unsigned int> > > & cstream_ids, nar::Database *db, nar::ServerGlobal* s_global){
            MessageTypes::FilePull::Response fpull_resp(200,s_global->randevous_port);

            for(int i=0;i<cTom.size();i++){
                nar::Chunk curChunk= db->getChunk(cTom[i].first);
                for(int j=0;j<cTom[i].second.size();j++){
                    struct MessageTypes::FilePull::Response::PeerListElement temp;
                    temp.machine_id = cTom[i].second[j].machine_id;
                    temp.chunk_id= curChunk.chunk_id;
                    temp.stream_id = cstream_ids[i].second[j];
                    temp.chunk_size = curChunk.chunk_size;
                    fpull_resp.add_element(temp);
                }
            }
            fpull_resp.send_mess(inf->getSck());
            return;
        }

        void file_pull(nar::SockInfo* inf, MessageTypes::FilePull::Request & file_pull_req, nar::Database* db ,nar::ServerGlobal* s_global) {
            bool errorFlag = false;
            string file_name = file_pull_req.get_filename();
            string dir = file_pull_req.get_dir();
            string user_name = inf->getUser();
            long long int file_id = findFileId(file_name, dir, user_name, db);
            int status_code = 200;
            if(inf->isAuthenticated())   {

                if(s_global->keepalives.size() < 2) {

                    status_code = 301; // no valid peer
                    MessageTypes::FilePull::Response file_pull_resp(status_code,0);
                    file_pull_resp.send_mess(inf->getSck());
                } else if(file_id != -1){

                    std::vector< long long int > pos_user = findPosUsers(file_id, db ,s_global);
                    nar::File desf = db->getFile(file_id);
                    std::vector<nar::Chunk> chunks = db->getChunks(file_id);
                    std::vector< std::pair<long long int, std::vector<unsigned int > > > cstream_ids;
                    std::vector< std::pair<long long int ,std::vector<nar::Machine> > > cTom;

                    for(int i=0;i<chunks.size();i++){

                        std::vector<nar::Machine> machines = db->getMachines(chunks[i].chunk_id);
                        std::pair<long long int ,std::vector<nar::Machine> > temp_pair(chunks[i].chunk_id,machines);
                        cTom.push_back(temp_pair);
                        std::vector<unsigned int> stream_ids;

                        for(int j =0;j<machines.size();j++){

                            unsigned int str_id = s_global->stream_id++;
                            stream_ids.push_back(str_id);
                            auto it = s_global->keepalives[machines[j].machine_id];
                            MessageTypes::WaitChunkPull::Request wpull_req((unsigned short)s_global->randevous_port,str_id,chunks[i].chunk_id,chunks[i].chunk_size);
                            MessageTypes::WaitChunkPull::Response wpull_resp(300);
                            wpull_req.send_mess(inf->getSck(),wpull_resp);
                        }

                        std::pair<long long int, std::vector<unsigned int> > temp_stream_ids(chunks[i].chunk_id,stream_ids);
                        cstream_ids.push_back(temp_stream_ids);


                    }
                    sendMachineListPull(inf,200, cTom,cstream_ids,  db,s_global);



                } else {

                    status_code = 303; // no valid peer
                    MessageTypes::FilePull::Response file_pull_resp(status_code,0);
                    file_pull_resp.send_mess(inf->getSck());
                }

            } else {

                status_code = 300; // no valid peer
                MessageTypes::FilePull::Response file_pull_resp(status_code,0);
                file_pull_resp.send_mess(inf->getSck());
            }
            return;

        }
    }
}

#endif
