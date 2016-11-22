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

using namespace nlohmann;

std::map<std::string, nar::Socket*> keepalives;
std::map<nar::Socket*, std::string> reverse_keepalives;
nar::Database db;

bool handshake(nar::Socket& skt, bool& keepalive) {
    std::string request = nar::get_message(skt);
    std::cout << "received: " << request << std::endl;
    auto jsn = json::parse(request.c_str());
    std::string action = jsn["header"]["action"];
    if(action != "handshake")
        return false;

    std::string username = jsn["payload"]["username"];
    if(keepalives.find(username) == keepalives.end()) {
        keepalive = true;
        keepalives[username] = &skt;
        reverse_keepalives[&skt] = username;
    } else {
        nar::Socket* keep_sck = keepalives[username];
        if(!keep_sck->is_active()) {
            keepalives[username] = &skt;
            reverse_keepalives[&skt] = username;
            std::map<nar::Socket*, std::string>::iterator it = reverse_keepalives.find(keep_sck);
            reverse_keepalives.erase(it);
            keepalive = true;
        }
    }

    json resp;
    resp["header"]["channel"] = "sp";
    resp["header"]["status-code"] = 200;
    resp["header"]["reply-to"] = "handshake";

    std::string response(resp.dump());
    std::string len = std::to_string((int)response.size());
    skt.send((char*) len.c_str(), (int) len.size());
    skt.send((char*) " ", 1);
    skt.send((char*) response.c_str(), (int)response.size());
    return true;
}

void handle_connection(nar::Socket skt, int id) {
    bool keepalive;
    if(handshake(skt, keepalive)) {
        do {
            std::string request = nar::get_message(skt);
            std::cout << request << std::endl;
        } while(keepalive);
        
    } else {
        skt.close();
    }
}

int main(int argc, char *argv[])
{
    db.setUser(std::string("root"));
    db.setPass(std::string("123"));
    db.setDbname(std::string("nar"));
    db.connect();
    nar::Socket entry_skt;
    entry_skt.create();
    entry_skt.bind(12345);
    entry_skt.listen();
    int i = 0;
    while(true) {
        nar::Socket new_skt;
        entry_skt.accept(new_skt, NULL);

        std::thread thr(&handle_connection, new_skt, i++);
        thr.detach();
    }    	
	return 0;
}
