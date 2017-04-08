#ifndef NAR_SERVERACTIONS_H
#define NAR_SERVERACTIONS_H
#include <nar/lib/Socket/Socket.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>

namespace nar {
    namespace ServerAction {
        void authenticate_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req, nar::Socket* skt);
        void register_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserRegister::Request& req, nar::Socket* skt);
        void machine_register_action(nar::ServerGlobal* s_global, nar::MessageTypes::MachineRegister::Request& req, nar::Socket* skt);
        //bool machine_authenticate_action(nar::ServerGlobal* s_global, nar::MessageTypes::MachineAuthenticationInit::Request& req, nar::Socket* skt);
    }
}

#endif
