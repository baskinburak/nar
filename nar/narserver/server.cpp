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
            token.push_back(charlist[std::rand()%charlist.size()]);
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

        bool file_push_request(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "file_push_request";
            if(inf->isAuthenticated()) {
                if(keepalives.size() == 0) {
                    resp["header"]["status-code"] = 301; // no valid peer
                } else {
                    std::map<std::string, nar::SockInfo*>::iterator it = keepalives.begin();
                    int selected_peer = std::rand() % ((int)keepalives.size());
                    std::advance(it, selected_peer);
                    int cnt = 0;
                    for(; (*it).first == inf->getAuthenticationHash() && cnt<keepalives.size(); it++, cnt++) {
                        if(it == keepalives.end())
                            it = keepalives.begin();
                    }

                    if(cnt == keepalives.size()) {
                        resp["header"]["status-code"] = 301; // no valid peer
                    } else {
                        json peer_msg;
                        peer_msg["header"]["channel"] = "sp";
                        peer_msg["header"]["action"] = "wait_chunk_request";
                        peer_msg["payload"]["token"] = generate_secure_token();
                        nar::SockInfo* peer_sock = (*it).second;
                        std::string peer_str(peer_msg.dump());
                        peer_str = std::to_string((int)peer_str.size()) + std::string(" ") + peer_str;
                        (peer_sock->getSck())->send((char*) peer_str.c_str(), (int)peer_str.size());


    
                        std::string peer_ip = (peer_sock->getSck())->get_dest_ip();


                        resp["header"]["status-code"] = 200;
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
        bool file_pull_request(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "file_pull_request";
            if(inf->isAuthenticated()) {
                if(keepalives.size() == 0) {
                    resp["header"]["status-code"] = 301; // no valid peer
                } else {
                    std::map<std::string, nar::SockInfo*>::iterator it = keepalives.begin();
                    int selected_peer = std::rand() % ((int)keepalives.size());
                    std::advance(it, selected_peer);
                    int cnt = 0;
                    for(; (*it).first == inf->getAuthenticationHash() && cnt<keepalives.size(); it++, cnt++) {
                        if(it == keepalives.end())
                            it = keepalives.begin();
                    }

                    if(cnt == keepalives.size()) {
                        resp["header"]["status-code"] = 301; // no valid peer
                    } else {
                        json peer_msg;
                        peer_msg["header"]["channel"] = "sp";
                        peer_msg["header"]["action"] = "wait_chunk_request";
                        peer_msg["payload"]["token"] = generate_secure_token();
                        nar::SockInfo* peer_sock = (*it).second;
                        std::string peer_str(peer_msg.dump());
                        peer_str = std::to_string((int)peer_str.size()) + std::string(" ") + peer_str;
                        (peer_sock->getSck())->send((char*) peer_str.c_str(), (int)peer_str.size());


    
                        std::string peer_ip = (peer_sock->getSck())->get_dest_ip();


                        resp["header"]["status-code"] = 200;
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
            nar::action::handshake(inf, jsn);
        } else if(jsn["header"]["action"] == "keepalive") {
            nar::action::keepalive(inf, jsn);
            break;
        } else if(jsn["header"]["action"] == "file_push_request") {
            nar::action::file_push_request(inf, jsn);
        } else if(jsn["header"]["action"] == "register") {
            nar::action::register_user(inf, jsn);
        }
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
