#ifndef NAR_ACTION_HANDSHAKE_H
#define NAR_ACTION_HANDSHAKE_H
#include <iostream>
#include <string>
#include <nar/lib/Messaging/MessageTypes/Handshake.h>
#include <nar/narserver/sockinfo.h>

namespace nar {
    namespace ServerActions {
        /*
         * Handshake functionality for server
         *
         *
         * @author: Fatih
         * @param: inf, nar::SockInfo* , holds receiving socket information
         * @param: hand_req, MessageTypes::Handshake::Request &, holds the received message'c class data
         * @tested: no
         * @todo: test
         * @return: void
        */
        void handshake(nar::SockInfo* inf, MessageTypes::Handshake::Request & hand_req) {
            std::string username = hand_req.get_username();
            std::string machine_id = hand_req.get_machine_id();
            inf->authenticate(username,machine_id);
            MessageTypes::Handshake::Response hand_resp(200);
            hand_resp.send_mess(inf->getSck());
            return;
        }
    }
}

#endif
