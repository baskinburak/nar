#include "ServerActions.h"
#include <boost/algorithm/string.hpp>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/lib/Socket/Socket.h>
#include <iostream>
#include <nar/narserver/Database.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>

#include <nar/narserver/sockinfo.h>
#include <nar/narserver/peers.h>


void nar::ServerAction::keepalive_action(nar::ServerGlobal* s_global, nar::MessageTypes::KeepAlive::Request& req, nar::Socket* skt) {
    std::string& mac_id = req.get_machine_id();
    nar::Peers* peers = s_global->peers;
    nar::Database* db = s_global->get_db();
    nar::DBStructs::Machine mac;
    try {
        mac = db->getMachine(mac_id);
    } catch(...) {
        std::cout<<"database error at keepalive"<<std::endl;
        nar::MessageTypes::KeepAlive::Response resp(405);
        try{
            resp.send_mess(skt);
        }
        catch(...) {
            std::cout<<"socket gone boom in keepalive 405"<<std::endl;
            skt->close();
            return;
        }
        skt->close();
        return;
    }

    std::vector<std::string> delete_list;
    try {
        if(mac.delete_list.size() > 0) {
            boost::split( delete_list, mac.delete_list, boost::is_any_of( "," ) );
            mac.delete_list = std::string();
            db->updateMachineDeleteList(mac);
        }
    } catch(...) {
        std::cout<<"boost or database error at server keepalive"<<std::endl;
        nar::MessageTypes::KeepAlive::Response resp(709);
        try{
            resp.send_mess(skt);
        }
        catch(...) {
            std::cout<<"socket gone boom in keepalive 709"<<std::endl;
            skt->close();
            return;
        }
        skt->close();
        return;
    }

    try {
        peers->insert_keepalive(mac_id, skt);
        nar::MessageTypes::KeepAlive::Response resp(200);
        resp.set_delete_list(delete_list);
        try{
            resp.send_mess(skt);
        }
        catch(...) {
            std::cout<<"socket gone boom in keepalive 200"<<std::endl;
            skt->close();
            return;
        }
    } catch(...) {
        nar::MessageTypes::KeepAlive::Response resp(300);
        try{
            resp.send_mess(skt);
        }
        catch(...) {
            std::cout<<"socket gone boom in keepalive 709"<<std::endl;
            skt->close();
            return;
        }
        skt->close();
        return;
    }
}
