
#include <map>
#include <nar/lib/Socket/Socket.h>
#include <thread>
#include <iostream>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/narnode/utility.h>
#include <nar/narnode/Exception/exception.h>
#include <nar/lib/json.hpp>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/sockinfo.h>
#include <cstdlib>
#include <ctime>
#include <iterator>

using namespace nlohmann;

std::map<std::string, nar::SockInfo*> keepalives;
nar::Database db;
std::map<std::string, bool> activetokens;
std::string charlist("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!#$%&'()*+,-./:;<=>?@[]^_`{|}~");

std::string generate_secure_token() {
    int TOKENLEN = 32;
    std::string token;
    token.resize(TOKENLEN);
    do {
        for(int i=0; i<TOKENLEN; i++) {
            token[i] = charlist[std::rand()%charlist.size()];
        }
    } while(activetokens.find(token) != activetokens.end());
    return token;
}

namespace nar {
    namespace action {
        bool handshake(nar::SockInfo* inf, json& jsn) {
            std::string username = jsn["payload"]["username"];

            inf->authenticate(username);

            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["status-code"] = 200;
            resp["header"]["reply-to"] = "handshake";

            std::string response(resp.dump());

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            return true;
        }

        bool keepalive(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "keepalive";
            if(inf->isAuthenticated()) {
                inf->elevateKeepalive();
                keepalives[inf->getAuthenticationHash()] = inf;
                resp["header"]["status-code"] = 200;
            } else {
                resp["header"]["status-code"] = 300;
            }

            std::string response(resp.dump());

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            return true;
        }

        bool getPeerPort(nar::SockInfo* active,std::string token){
            std::cout << "IMA1" << std::endl;
            json req;
            req = json::parse(get_message( *(active->getSck())) );
                        std::cout << "IMA2" << std::endl;
            std::string peerId = req["payload"]["peer-id"];
                    std::cout << "IMA3" << std::endl;
            nar::SockInfo *peerSck = keepalives[peerId];
                        std::cout << "IMA4" << std::endl;

            json reqR;
            reqR["header"]["channel"] = "sp";
            reqR["header"]["action"] = "peer_port_request";
            reqR["payload"]["token"] = token;
            send_message( peerSck->getSck() , reqR.dump());
            std::cout << "IMA5" << std::endl;
            json respR;
            std::string tmp = get_message( *(peerSck->getSck()));
            std::cout << tmp << std::endl;
            respR = json::parse(tmp );
            std::cout << "IMA6" << std::endl;
            int peerPort = respR["payload"]["port"];
            std::cout << "IMA7" << std::endl;

            json respA;
            respA["header"]["reply-to"] = "peer_connection_request";
            respA["header"]["channel"] = "sp";
            respA["header"]["status-code"] = 200;
            respA["payload"]["peer-ip"] = (peerSck->getSck())->get_dest_ip();
            respA["payload"]["peer-id"] = peerId;
            respA["payload"]["peer-port"] = peerPort;
            send_message( active->getSck() , respA.dump());
        }

        bool sendPeerList(nar::SockInfo* inf, int status, unsigned long cSize, int fId, std::string peerId, std::string cId, std::string token){
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "file_push_request";
            resp["header"]["status-code"] = 200;
            resp["payload"]["chunk-size"] = cSize;
            resp["payload"]["file-id"] = fId;
            json arr;
            arr["peer_id"] = peerId;
            arr["chunk_id"] = cId;
            arr["token"] = token;

            resp["payload"]["peer-list"] = { arr };
            std::string rspMsg(resp.dump());
            send_message( inf->getSck() , rspMsg);
            return true;
        }

        nar::File insertFileToDb(std::string fName,unsigned long fSize, std::string fDir,std::string uname) {
            nar::File fi;
            fi.file_name = fName;
            fi.file_size = fSize;


            ::db.insertFile(fi);
            long long int f_id = ::db.getNextFileId()-1;
            nar::Directory dir = ::db.findDirectoryId(uname,fDir);
            nar::DirectoryTo dt;
            dt.dir_id = dir.dir_id;
            dt.item_id = f_id;
            dt.ForD = 0;
            std::cout<<f_id<<std::endl;
            ::db.insertDirectoryTo(dt);
            nar::File output = ::db.getFile(f_id);

            std::cout<<"file a "<<output.file_name<<" "<<output.file_id<<std::endl;
            return output;
            //nar::User user = ::db.getUser(nar::globals->get_username());

        }
        long long int findFileId(std::string& file_name,std::string& dir_name,std::string& uname){// returns -1 in any case of problem
            long long int file_id = -1;
            nar::Directory dir = ::db.findDirectoryId(uname,dir_name);
            if(dir.id == -1){
                std::cout<<"No such User directory pair"<<std::endl;
            } else{
                std::vector<nar::File> files = ::db.getDirectoryFile(dir.dir_id);
                for(int i= 0;i<files.size();i++){
                    if(files[i].file_name.compare(file_name)== 0){
                        file_id = files[i].file_id;
                    }
                }
            }
            return file_id;

        }
        std::vector<long long int> findPosUsers(long long int  f_id){
            std::vector<nar::User> users = ::db.getUserFromFile(file_id);
            std::vector<long long int> output;
            for(int i= 0;i<users.size();i++){
                auto it = keepalives.find(users[i].user_name);
                if(it!= keepalives.end()){
                    output.push_back(users[i].user_id);
                }
            }
            return output;


        }

        bool file_push_request(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "file_push_request";
            std::string fName = jsn["payload"]["file-name"];
            unsigned long fSize = jsn["payload"]["file-size"];
            std::string fDir = jsn["payload"]["directory"];
            if(inf->isAuthenticated()) {
                if(keepalives.size() < 2) {
                                std::cout << "Here2" << std::endl;
                    resp["header"]["status-code"] = 301; // no valid peer
                } else {

                    nar::File file = insertFileToDb(fName,fSize,fDir,inf->getAuthenticationHash());
                    auto it=keepalives.begin();
                    int selected_peer;
                    do{
                        selected_peer = std::rand() % ((int)keepalives.size());
                        std::cout<<selected_peer<<std::endl;
                        std::advance(it, selected_peer);
                    }
                    while(it->first.compare(inf->getAuthenticationHash())==0);

                    nar::User us = ::db.getUser(it->first);
                    std::cout<<"name to "<<it->first<<" "<<us.user_name<<" "<<us.user_id<<" "<<inf->getAuthenticationHash()<<std::endl;
                    nar::UserToFile ust;
                    ust.user_id = us.user_id;
                    ust.file_id = file.file_id;
                    std::cout<<"file _ name "<<file.file_id<<" "<<file.file_name<<std::endl;
                    ::db.insertUserToFile(ust);
                    int cnt = 0;
                    if(cnt == keepalives.size()) {
                        std::cout << "Here4" << std::endl;
                        resp["header"]["status-code"] = 301; // no valid peer
                    } else {
                        std::string token =  generate_secure_token();

                        json peer_msg;
                        peer_msg["header"]["channel"] = "sp";
                        peer_msg["header"]["action"] = "wait_chunk_push_request";
                        peer_msg["payload"]["token"] = token;
                        std::cout << token  << " " << token.size() << std::endl;
                        peer_msg["payload"]["chunk-id"] = std::to_string(file.file_id);
                        peer_msg["payload"]["chunk-size"] = fSize;


                        nar::SockInfo* peer_sock = (*it).second;
                        std::string peer_str(peer_msg.dump());

                        std::cout << "\n"<< peer_str << std::endl;
                        send_message( peer_sock->getSck() , peer_str);                        std::cout << "yama" << std::endl;
                        json rspX = json::parse(get_message( *(peer_sock->getSck())) );                         std::cout << "yama" << std::endl;
                        std::cout << rspX << std::endl;


                        sendPeerList( inf, 200 , file.file_size,  file.file_id,  it->first, std::to_string(file.file_id), token);
                        std::cout << "prrrt" << std::endl;

                        getPeerPort(inf,token);

                        /*

                        resp["header"]["status-code"] = 200;
                        std::string filename = jsn["payload"]["file-name"];
                        unsigned long filesize = jsn["payload"]["file-size"];
                        std::string directory = jsn["payload"]["directory"];
                        std::cout << filename << std::endl;
                        std::cout << filesize << std::endl;
                        std::cout << directory << std::endl << std::endl;*/



                    }
                }

            } else {
                resp["header"]["status-code"] = 300;
                            std::cout << "Here6" << std::endl;
            }
                        std::cout << "Here7" << std::endl;
            std::string response(resp.dump());
            //while(1);

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            return true;

        }
        bool file_pull_request(nar::SockInfo* inf, json& jsn) {
            std::string token = generate_secure_token();
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "file_pull_request";
            std::string file_name = jsn["payload"]["file_name"];
            std::string dir = jsn["payload"]["directory"];
            std::string user_name = inf->getAuthenticationHash();
            long long int file_id = findFileId(file_name,dir,user_name);

            if(inf->isAuthenticated())   {
                if(keepalives.size() < 2) {
                    resp["header"]["status-code"] = 301; // no valid peer
                } else if(file_id != -1){
                    std::vector< long long int > pos_user = findPosUsers(file_id);
                    if(pos_user.size()==0){
                        resp["header"]["status-code"] = 304 // There is no peer with required file
                        std::cout<<" There is no user with the required file try push first "<<std::endl;
                    } else {
                            nar::File desf = ::db.getFile(file_id);
                            auto it = keepalives.begin();
                            json peer_msg;
                            peer_msg["header"]["channel"] = "sp";
                            peer_msg["header"]["action"] = "wait_chunk_pull_request";

                            peer_msg["payload"]["token"] = token;
                            peer_msg["payload"]["chunk-id"] = std::to_string(file_id);
                            peer_msg["payload"]["chunk-size"] = desf.file_size;
                            std::vector< std::map<std::string, nar::SockInfo*>::iterator > send_peers;
                            for(int i=0;i<pos_user.size();i++){
                                nar::User temp_user = ::db.getUser(pos_user[i]);
                                auto it_temp = keepalives.find(temp_user.user_name);
                                send_peers.push_back(it_temp);
                                std::string temp_peer_str(peer_msg.dump());
                                send_message(*((*it_temp).second->getSck()), temp_peer_str);
                                std::string temp_str_come = get_message(*((*it_temp).second->getSck()) );
                            }



                            json real_peer;
                            real_peer["header"]["channel"] = "sp";
                            real_peer["header"]["status-code"] = 200;
                            real_peer["header"]["action"] = "file_pull_request";

                            real_peer["payload"]["file-id"] = file_id;
                            real_peer["payload"]["peer-list"] = json::array();
                            real_peer["payload"]["chunk-size"] = desf.file_size;
                            std::cout << "pls7" << std::endl;
                            for(int i=0;i<pos_user.size();i++){
                                    json per_peer;
                                    nar::User temp_us = ::db.getUser(pos_user[i]);
                                    per_peer["peer_id"] = temp_us.user_name;
                                    per_peer["chunk_id"] = std::to_string(file_id);
                                    per_peer["token"] = token;
                                    real_peer["payload"]["peer-list"][i] = per_peer;


                            }

            //////////////////////////////////////////////////////////////////////////////////////CHHHHHHHHHHHHHHHHHHHHHHHHAAAAAAAAAAAANNNNNNNNNNNNGGGGGGGGGGGEEEEEEEEEEEEEEEe
                            send_message(*(inf -> getSck()), real_peer.dump());

                            std::string strcome = get_message(*(inf -> getSck()));
                            std::cout << "str: " << strcome << std::endl;

                            json jreq2 = json::parse(strcome);
                            std::string peerId = jreq2["payload"]["peer-id"];
                            jreq2.clear();
                            jreq2["header"]["channel"] = "sp";
                            jreq2["header"]["action"] = "peer_port_request_pull";
                            jreq2["payload"]["token"] = token;
                            for(int i= 0;i<send_peers.size();i++){
                                if(send_peers[i]->first.compare(peerId) == 0){
                                    std::string temp_jreq(jreq2.dump());
                                    send_message(*((*send_peers[i]).second->getSck()), temp_jreq);
                                    std::string temp_str_come = get_message(*((*send_peers[i]).second->getSck()) );
                                    json temp_json = json::parse(temp_str_come);
                                    int peerPort = temp_json["payload"]["port"];
                                    std::string peer_ip = ((*send_peers[i]).second->getSck())->get_dest_ip();
                                    json bull;
                                    bull["header"]["channel"] = "sp";
                                    bull["header"]["status-code"] = 200;
                                    bull["header"]["reply-to"] = "peer_connection_request";
                                    bull["payload"]["peer-ip"] = peer_ip;
                                    bull["payload"]["peer-port"] = peerPort;
                                    bull["payload"]["peer-id"] = send_peers[i]->first;
                                    send_message(*(inf->getSck()),bull.dump());
                                }
                            }

                    }
                } else {
                    resp["header"]["status-code"] = 303;
                }

            } else {
                resp["header"]["status-code"] = 300;
            }

            std::string response(resp.dump());

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            return true;

        }
        bool register_user(nar::SockInfo* inf, json& jsn) {
            /*
                {
                    "header": {
                        "channel": "sp",
                        "reply-to": "register",
                        "status-code": ???
                    }, "payload": {
                    }
                }
            */
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "register";


            std::string username = jsn["payload"]["username"];
            std::string aes = jsn["payload"]["aes"];
            nar::User usr = ::db.getUser(username);
            if(usr.user_id != -1) {
                resp["header"]["status-code"] = 301; // username already exists
                nar::send_message(*(inf->getSck()), std::string(resp.dump()));
                return false;
            }

            usr.user_name = username;
            usr.quota = 0;
            usr.disk_space = 0;
            usr.cryptedKey = aes;
            ::db.insertUser(usr);
            resp["header"]["status-code"] = 200; // success
            nar::send_message(*(inf->getSck()), std::string(resp.dump()));
            return true;
        }
        bool get_user_dir_info(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "dir_info";
            if(inf->isAuthenticated()) {
                std::string user_name = inf->getAuthenticationHash();
                std::string dir_name =  jsn["payload"]["dir_name"].get<std::string>();
                nar::User us= ::db.getUser(user_name);
                std::vector<nar::File> files;
                std::vector<nar::Directory> dirs;
                if(dir_name.compare(std::string("")) == 0){
                    files = ::db.getDirectoryFile(us.dir_id);
                    dirs = ::db.getDirectoryDir(us.dir_id);
                } else {
                    nar::Directory dir = ::db.findDirectoryId(user_name,dir_name);
                    if(dir.dir_id != -1){
                        files = ::db.getDirectoryFile(dir.dir_id);
                        dirs = ::db.getDirectoryDir(dir.dir_id);
                    }
                    else {
                        resp["header"]["status-code"] = 302;
                    }
                }
                if(files.size() == 0){
                    //resp["payload"]["file_list"] = []
                }
                else {
                    json holder;
                    for(int i = 0;i<files.size();i++) {

                        std::string a = std::string("file")+std::to_string(i);
                        holder[a]["file_id"] = files[i].file_id;
                        holder[a]["file_name"] = files[i].file_name;
                        holder[a]["file_size"] = files[i].file_size;
                        holder[a]["file_change_time"] = files[i].change_time;
                        //std::cout<<files[i].change_time<<std::endl;
                        //resp["payload"]["file_list"] = [];

                    }
                    resp["payload"]["file_list"] = holder ;
                }
                if(dirs.size() == 0){
                    //resp["payload"]["file_list"] = []
                }
                else {
                    json holder;
                    for(int i = 0;i<dirs.size();i++) {

                        std::string a = std::string("dir")+std::to_string(i);
                        holder[a]["dir_id"] = dirs[i].dir_id;
                        holder[a]["dir_name"] = dirs[i].dir_name;
                        holder[a]["dir_size"] = dirs[i].dir_size;
                        holder[a]["dir_change_time"] = dirs[i].change_time;
                        //resp["payload"]["file_list"] = [];

                    }
                    resp["payload"]["dir_list"] = holder ;
                }
                std::string  test = resp.dump();
std::cout<<test<<std::endl;


            } else {
                resp["header"]["status-code"] = 301;
            }
            std::string response(resp.dump());

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            std::cout<<"success"<<std::endl;
            return true;
       }

       bool get_aes_key(nar::SockInfo* inf, json& jsn) {

            std::string user_name = inf->getAuthenticationHash();
            nar::User usr = ::db.getUser(user_name);
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["status-code"] = 200;
            resp["header"]["reply-to"] = "get_aes_request";
            resp["payload"]["aes"] = usr.cryptedKey;
            std::cout << "uname: " << usr.user_name << " "  << "key: "<< usr.cryptedKey << std::endl;
            std::cout << "sname: " << inf->getAuthenticationHash() << std::endl;
            nar::send_message(*(inf->getSck()), std::string(resp.dump()));
               return true;
       }

    }
}

void handle_connection(nar::Socket* skt) {
    nar::SockInfo* inf = new nar::SockInfo(skt);
    while(true) {
        std::string msg = nar::trim(nar::get_message(*skt));
        std::cout << msg << std::endl;
        auto jsn = json::parse(msg.c_str());
        std::cout << "zu" << std::endl;
        std::cout << jsn["header"]["action"] << std::endl;
        if(jsn["header"]["action"] == "handshake") {
            std::cout<<"<hand"<<std::endl;
            nar::action::handshake(inf, jsn);
            std::cout<<"hand>"<<std::endl;
        } else if(jsn["header"]["action"] == "keepalive") {
            std::cout<<"<alive"<<std::endl;
            nar::action::keepalive(inf, jsn);
            std::cout<<"alive>"<<std::endl;
            break;
        } else if(jsn["header"]["action"] == "file_push_request") {
                        std::cout << "Here8" << std::endl;
            nar::action::file_push_request(inf, jsn);
        } else if(jsn["header"]["action"] == "file_pull_request") {
            nar::action::file_pull_request(inf, jsn);
        } else if(jsn["header"]["action"] == "register") {
            nar::action::register_user(inf, jsn);
        } else if(jsn["header"]["action"] == "dir_info") {
            std::cout<<"<ls"<<std::endl;
            nar::action::get_user_dir_info(inf, jsn);
            std::cout<<"ls>"<<std::endl;
        } else if(jsn["header"]["action"] == "get_aes_request") {
            std::cout<<"<aes"<<std::endl;
            nar::action::get_aes_key(inf, jsn);
            std::cout<<"aes>"<<std::endl;

        }
                    std::cout << "Here9" << std::endl;
    }

    if(inf->isKeepalive()) {
    } else {
        (inf->getSck())->close();
    }
}

int main(int argc, char *argv[])
{
    std::srand(std::time(NULL));
    db.setUser(std::string("root"));
    db.setPass(std::string("123"));
    db.setDbname(std::string("nar"));
    db.connect();
    nar::Socket entry_skt;
    entry_skt.create();
    entry_skt.bind(12345);
    entry_skt.listen();
    while(true) {
        nar::Socket* new_skt = new nar::Socket();
        entry_skt.accept(*new_skt, NULL);

        std::thread thr(&handle_connection, new_skt);
        thr.detach();
    }
    return 0;
}
