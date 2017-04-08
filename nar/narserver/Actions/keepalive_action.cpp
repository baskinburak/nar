#include "ServerActions.h"
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/lib/Socket/Socket.h>

void nar::ServerAction::keepalive_action(nar::ServerGlobal* s_global, nar::MessageTypes::KeepAlive::Request& req, nar::Socket* skt) {
    
}
