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
        bool get_user_dir_info(nar::SockInfo* inf, json& jsn) {
            json resp;
            resp["header"]["channel"] = "sp";
            resp["header"]["reply-to"] = "get_user_dir_info";
            if(inf->isAuthenticated()) {
                std::string user_name = resp["payload"]["user_name"].get<std::string>();
                std::string dir_name =resp["payload"]["dir_name"].get<std::string>();
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



            } else {
                resp["header"]["status-code"] = 301;
            }
            std::string response(resp.dump());

            response = std::to_string((int)response.size()) + std::string(" ") + response;
            (inf->getSck())->send((char*) response.c_str(), (int)response.size());
            return true;
       }
    }
}

void handle_connection(nar::Socket* skt) {
    nar::SockInfo* inf = new nar::SockInfo(skt);
    while(true) {
        std::string msg = nar::get_message(*skt);
        std::cout << msg << std::endl;
        auto jsn = json::parse(msg.c_str());
        std::cout << jsn["header"]["action"] << std::endl;
        if(jsn["header"]["action"] == "handshake") {
            nar::action::handshake(inf, jsn);
        } else if(jsn["header"]["action"] == "keepalive") {
            nar::action::keepalive(inf, jsn);
            break;
        } else if(jsn["header"]["action"] == "file_push_request") {
            nar::action::file_push_request(inf, jsn);
        } else if(jsn["header"]["action"] == "get_user_dir_info") {
             nar::action::get_user_dir_info(inf, jsn);
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
