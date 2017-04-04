#ifndef NAR_SERVERACTIONS_H
#define NAR_SERVERACTIONS_H

namespace nar {
    namespace ServerAction {
        void authenticated_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req);
    }
}

#endif
