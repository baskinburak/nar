#include "ServerActions.h"
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/lib/Socket/Socket.h>
#include <iostream>
#include <nar/narserver/Database.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/sockinfo.h>
#include <nar/narserver/peers.h>


void nar::ServerAction::keepalive_action(nar::ServerGlobal* s_global, nar::MessageTypes::KeepAlive::Request& req, nar::Socket* skt) {
    std::cout << "here i am" << std::endl;
    std::string& mac_id = req.get_machine_id();
    std::cout << "here i am2" << std::endl;
    nar::Peers* peers = s_global->peers;
    std::cout << "here i am3" << std::endl;
    peers->insert_keepalive(mac_id, skt);
    std::cout << "here i am4" << std::endl;

    nar::MessageTypes::KeepAlive::Response resp(200);
    std::cout << "here i am5" << std::endl;
    resp.send_mess(skt);
    std::cout << "here i am6" << std::endl;
}
