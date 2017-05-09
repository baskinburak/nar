//
// Created by fatih on 09.05.2017.
//

#include "ServerActions.h"

void nar::ServerAction::daemon_shutdown_action(nar::ServerGlobal *s_global, nar::MessageTypes::DaemonShutdown::Request &req,nar::Socket* skt) {

    nar::Peers* peers = s_global->peers;
    std::string& mac_id = req.get_machine_id();
    try {

        peers->delete_keepalive(mac_id);
    } catch(...) {
        std::cout<<"daemon_shutdown_action peer delete_keepalive failure"<<std::endl;
        skt->close();
    }
}
