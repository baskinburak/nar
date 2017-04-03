#ifndef NAR_DAEMON_HANDSHAKE_H
#define NAR_DAEMON_HANDSHAKE_H
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/Messaging/MessageTypes/Handshake.h>

namespace nar {
    namespace DaemonTask {
        bool handshake(nar::Socket& skt, nar::Global* globals);
    }
}

#endif
