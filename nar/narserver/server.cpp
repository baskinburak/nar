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
            long long int f_id = ::db.getNextFileId();
			::db.insertFile(fi);
            nar::Directory dir = ::db.findDirectoryId(uname,fDir);
            nar::DirectoryTo dt;
            dt.dir_id = dir.dir_id;
            dt.item_id = f_id;
            dt.ForD = 0;
            ::db.insertDirectoryTo(dt);
            nar::User us = ::db.getUser(uname);
            nar::UserToFile ust;
            ust.user_id = us.user_id;
            ust.file_id = f_id;
            ::db.insertUserToFile(ust);
            nar::File output = ::db.getFile(f_id);
            return output;
			//nar::User user = ::db.getUser(nar::globals->get_username());

		}
        std::vector<long long int> findPosUsers(std::string& file_name,std::string& dir_name,std::string& uname,long long int & f_id){
            nar::Directory dir = ::db.findDirectoryId(uname,dir_name);
            std::vector<nar::File> files = ::db.getDirectoryFile(dir.dir_id);
            long long int file_id;
            for(int i= 0;i<files.size();i++){
                if(files[i].file_name.compare(file_name)== 0){
                    file_id = files[i].file_id;
                }
            }
            f_id = file_id;
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
                    auto it = keepalives.begin();
                    int selected_peer = std::rand() % ((int)keepalives.size()-1);
                    std::advance(it, selected_peer);
                    int cnt = 0;
					for(; it->first == inf->getAuthenticationHash(); it++) {				// COMPLEXITY?
						if(  std::distance( it, keepalives.end() ) == 1 ) {
							it = keepalives.begin();
						}
					}
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

						auto it2 = keepalives.begin();
						for(;it2 != keepalives.end(); ++it2) {
							if(it2->first != inf->getAuthenticationHash())
								break;
						}

	                    nar::SockInfo* peer_sock = (*it2).second;
						std::string peer_str(peer_msg.dump());

						std::cout << "\n"<< peer_str << std::endl;
						send_message( peer_sock->getSck() , peer_str);						std::cout << "yama" << std::endl;
						json rspX = json::parse(get_message( *(peer_sock->getSck())) ); 						std::cout << "yama" << std::endl;
						std::cout << rspX << std::endl;


						sendPeerList( inf, 200 , file.file_size,  file.file_id,  it2->first, std::to_string(file.file_id), token);
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

            if(inf->isAuthenticated())   {
                if(keepalives.size() == 0) {
                    resp["header"]["status-code"] = 301; // no valid peer
                } else {
                    long long int file_id;
                    std::string user_name = inf->getAuthenticationHash();
                    std::vector< long long int > pos_user = findPosUsers(file_name,dir,user_name,file_id);
                    nar::File desf = ::db.getFile(file_id);
                    auto it = keepalives.begin();
                    int selected_peer = std::rand() % ((int)keepalives.size()-1);
                    std::advance(it, selected_peer);
                    std::cout << (*it).first << std::endl;
                    int cnt = 0;

                    for(; (*it).first == inf->getAuthenticationHash() ; it++) {

                        if(std::distance(it, keepalives.end()) == 1)
                        {
                            it = keepalives.begin();
                        }
                    }

                    std::cout << "cnt: " << cnt << std::endl;
                    std::cout << "keepalivesSize:" << keepalives.size() << std::endl;

                    if(cnt == keepalives.size()) {
                        resp["header"]["status-code"] = 301; // no valid peer
                        std::cout << "Error 301 attim!" << std::endl;
                    } else {
                        json peer_msg;
                        peer_msg["header"]["channel"] = "sp";
                        peer_msg["header"]["action"] = "wait_chunk_pull_request";

                        peer_msg["payload"]["file-id"] = file_id;
                        peer_msg["payload"]["peer-list"] = json::array();
                        for(int i=0;i<pos_user.size();i++){
                                json per_peer;
                                per_peer["peer_id"] = pos_user[i];
                                per_peer["chunk_size"] = desf.file_size;
                                per_peer["chunk_id"] = std::to_string(file_id);
                                per_peer["token"] = token;
                                peer_msg[i] = per_peer;


                        }









                        nar::SockInfo* peer_sock = (*it).second;
                        std::string peer_str(peer_msg.dump());
                        //peer_str = std::to_string((int)peer_str.size()) + std::string(" ") + peer_str;
                        std::cout << "amk4" << std::endl;

                        //(peer_sock->getSck())->send((char*) peer_str.c_str(), (int)peer_str.size());
                        send_message(*(peer_sock->getSck()), peer_str);
                        std::string strcome3 = get_message(*(peer_sock->getSck()) );
                        std::cout << "amk5" << std::endl;

                        std::cout << "gogogo" << std::endl;
                        //while(1);

                        //std::string peer_ip = (peer_sock->getSck())->get_dest_ip();

                        std::cout << "amk6" << std::endl;


                        json jreq;
                        jreq["header"]["channel"] = "sp";
                        jreq["header"]["status-code"] = 200;
                        jreq["header"]["reply-to"] = "file_pull_request";
                        jreq["payload"]["chunk-size"] = desf.file_size;

                        json tmp;
                        /*std::map<std::string, nar::SockInfo*>::iterator it2 = keepalives.begin();
                        for(;it2 != keepalives.end() ; it2++){
                            if(it2->first != inf -> getAuthenticationHash())
                            {
                                break;
                            }
                        }*/

                        tmp["peer_id"] = (it->first);
                        tmp["chunk_id"] = std::to_string(desf.file_id);
                        tmp["token"] = token;

                        jreq["payload"]["peer-list"] = { tmp };

                        send_message(*(inf -> getSck()), jreq.dump());

                        std::string strcome = get_message(*(inf -> getSck()));
                        std::cout << "str: " << strcome << std::endl;

                        json jreq2 = json::parse(strcome);
                        std::string peerId = jreq2["payload"]["peer-id"];
                        jreq2.clear();
                        jreq2["header"]["channel"] = "sp";
                        jreq2["header"]["action"] = "peer_port_request_pull";
                        jreq2["payload"]["token"] = token;

                        send_message(*(peer_sock->getSck()), jreq2.dump());

                        std::string strcome2 = get_message(*(peer_sock->getSck()));
                        jreq.clear();
                        jreq = json::parse(strcome2);
                        std::string peer_ip = (peer_sock->getSck())->get_dest_ip();
                        int peerPort = jreq["payload"]["port"];
                        jreq.clear();
                        jreq["header"]["channel"] = "sp";
                        jreq["header"]["status-code"] = 200;
                        jreq["header"]["reply-to"] = "peer_connection_request";
                        jreq["payload"]["peer-ip"] = peer_ip;
                        jreq["payload"]["peer-port"] = peerPort;
                        jreq["payload"]["peer-id"] = it->first;
                        send_message(*(inf->getSck()),jreq.dump());
                        while(10);

                        std::string filename = jsn["payload"]["file-name"];
                        unsigned long filesize = jsn["payload"]["file-size"];
                        std::string directory = jsn["payload"]["directory"];
                        std::cout << filename << std::endl;
                        std::cout << filesize << std::endl;
                        std::cout << directory << std::endl << std::endl;
                    }
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
            resp["header"]["reply-to"] = "get_user_dir_info";
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
        } else if(jsn["header"]["action"] == "get_user_dir_info") {
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
