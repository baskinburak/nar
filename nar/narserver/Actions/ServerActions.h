#ifndef NAR_SERVERACTIONS_H
#define NAR_SERVERACTIONS_H
#include <nar/lib/Socket/Socket.h>

namespace nar {
    namespace ServerAction {
        void authenticated_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req, nar::Socket* skt);
    }
}

#endif
