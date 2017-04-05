#ifndef NAR_AUTHACTIONS_H
#define NAR_AUTHACTIONS_H
#include <nar/lib/Socket/Socket.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>

namespace nar {
    namespace AuthAction {
        void authentication_dispatcher(nar::ServerGlobal* s_global, nar::Socket* skt,nar::DBStruct::User& user);
        void push_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageType::Request& req, nar::DBStruct::User&  user);
    }
}

#endif
