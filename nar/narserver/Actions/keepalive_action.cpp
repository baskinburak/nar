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
    std::string& mac_id = req.get_machine_id();
    nar::Peers* peers = s_global->peers;
    try {
        peers->insert_keepalive(mac_id, skt);
        nar::MessageTypes::KeepAlive::Response resp(200);
        resp.send_mess(skt);
    } catch(...) {
        nar::MessageTypes::KeepAlive::Response resp(300);
        resp.send_mess(skt);
        skt->close();
    }
}
