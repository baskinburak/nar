#ifndef NAR_ACTION_KEEPALIVE_H
#define NAR_ACTION_KEEPALIVE_H

#include <iostream>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/narserver/sockinfo.h>
#include <nar/narserver/ServerGlobal.h>
#include <string>
namespace nar {
    namespace Actions {
        void keepalive(nar::SockInfo* inf, nar::MessageTypes::KeepAlive::Request & keep_req, nar::ServerGlobal* s_global) {
            int status;
            if(inf->isAuthenticated()) {
                inf->elevateKeepalive();
                s_global->keepalives[inf->getAuthenticationHash()] = inf;
                status = 200;
            } else {
                status = 300;
            }
            nar::MessageTypes::KeepAlive::Response keep_resp(status);
            keep_resp.send_mess(inf->getSck());
            return ;
        }
    }
}


#endif
