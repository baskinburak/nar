#ifndef NAR_ACTION_FILE_PUSH_H
#define NAR_ACTION_FILE_PUSH_H

#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>
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

        void sendMachineListPush(nar::SockInfo* inf, int status, unsigned long csize, std::vector<std::string> & machine_list, long long int cId,std::vector<unsigned int> & stream_ids,long long int fSize, nar::ServerGlobal* s_global){

            int status_code = 200;
            std::vector<struct nar::MessageTypes::FilePush::Response::PeerListElement> holder;
            for(int i=0 ;i<machine_list.size();i++){

                if (fSize < csize) {
                    csize= fSize;
                } else {
                    fSize -= csize;
                }
                struct nar::MessageTypes::FilePush::Response::PeerListElement temp;
                temp.machine_id = machine_list[i];
                temp.chunk_id = cId+i;
                temp.stream_id = stream_ids[i];
                temp.chunk_size = csize;
                holder.push_back(temp);
            }
            nar::MessageTypes::FilePush::Response fpush_resp(status_code, s_global->randevous_port, holder);
            fpush_resp.send_mess(inf->getSck());
            return ;
        }


        nar::File insertFileToDb(string fName,unsigned long fSize, string fDir,string uname ,nar::Database* db ) {
            nar::File fi;
            fi.file_name = fName;
            fi.file_size = fSize;
            long long int f_id = db->getNextFileId();
            if(f_id == -1) {
                f_id = 1;
            }

            db->insertFile(fi);

            nar::Directory dir = db->findDirectoryId(uname,fDir);
            nar::DirectoryTo dt;
            dt.dir_id = dir.dir_id;
            dt.item_id = f_id;
            dt.ForD = 0;
            db->insertDirectoryTo(dt);
            nar::File output = db->getFile(f_id);

            return output;

        }

        void insertFileAndChunks(string username,std::vector<string> &selected_machines,long long int cId,string fName,long long int fSize,string fDir,long long int csize ,nar::Database* db ){
            nar::File f = insertFileToDb(fName,fSize,fDir,username,db);
            long long int fId = f.file_id;
            for(int i= 0;i<selected_machines.size();i++){

                nar::Chunk tempChunk;

                if (fSize < csize) {
                    csize= fSize;
                } else {
                    fSize -= csize;
                }
                tempChunk.chunk_size = csize;
                tempChunk.file_id = fId;
                db->insertChunk(tempChunk);
                nar::ChunkToMachine tempctm;
                tempctm.chunk_id = cId+i;
                tempctm.machine_id = selected_machines[i];
                db->insertChunkToMachine(tempctm);
            }

        }


        void file_push(nar::SockInfo* inf, MessageTypes::FilePush::Request & file_push_req, nar::Database* db ,nar::ServerGlobal* s_global) {

            string fName = file_push_req.get_filename();
            long long int fSize = file_push_req.get_filesize();
            string fDir = file_push_req.get_dir();
            long int onemb = 1024*1024*20;
            int status_code;
            if(inf->isAuthenticated()) {
                if(s_global->keepalives.size() < 2) {
                    status_code = 301; // no valid peer
                    nar::MessageTypes::FilePush::Response file_push_resp(status_code,0);
                    file_push_resp.send_mess(inf->getSck());
                } else {

                    int maxnum= fSize/onemb;
                    if( fSize % onemb !=0 ) maxnum++;
                    std::vector<string> selected_machines;
                    int selected_machine;
                    do{
                        auto it=s_global->keepalives.begin();
                        selected_machine = std::rand() % ((int)s_global->keepalives.size());
                        cout<<"selected number "<<selected_machine<<endl;
                        std::advance(it, selected_machine);
                        if(it->second->getUser().compare(inf->getUser())==0) {
                            continue;
                        } else {
                            selected_machines.push_back(it->second->getAuthenticationHash());
                        }

                    }
                    while(selected_machines.size()<maxnum);

                    std::vector<unsigned int> stream_ids;
                    long long int  cId= db->getNextChunkId();
                    if(cId== -1){
                        cId = 1;
                    }

                    for(int i = 0;i<selected_machines.size();i++){
                        stream_ids.push_back(s_global->stream_id++);
                    }
                    long long int altfSize = fSize;
                    long long int  csize = onemb;
                    cout<<"here 7"<<endl;
                    insertFileAndChunks(inf->getUser(),selected_machines,cId,fName,fSize,fDir,onemb,db);
                    cout<<"here 8"<<endl;
                    for(int i = 0;i<selected_machines.size();i++){
                        if (altfSize < csize) {
                            csize= altfSize;
                        } else {
                            altfSize -= csize;
                        }

                        long long int sid = stream_ids[i];
                        long long int cid = cId+i;
                        long long int cs = csize;
                        unsigned short rport = (unsigned short) s_global->randevous_port;
                        nar::MessageTypes::WaitChunkPush::Request wpush_req(rport,sid,cid,cs);
                        nar::MessageTypes::WaitChunkPush::Response wpush_resp(200);

                        nar::SockInfo* peer_sock = s_global->keepalives[selected_machines[i]];
                        wpush_req.send_mess(peer_sock->getSck(),wpush_resp);
                        if(wpush_resp.get_status_code() != 200) {
                            cout<< "there is a problem"<<endl;
                        }


                    }
                    cout<<"here 9"<<endl;
                    sendMachineListPush( inf, 200, onemb, selected_machines ,cId, stream_ids , fSize, s_global);
                    cout<<"here 10"<<endl;
                }

            } else {
                status_code = 302;
                nar::MessageTypes::FilePush::Response file_push_resp(status_code,0);
                file_push_resp.send_mess(inf->getSck());
            }

            return;

        }
    }
}

#endif
