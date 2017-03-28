#ifndef NAR_DAEMON_ITASK_H
#define NAR_DAEMON_ITASK_H
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>

namespace nar {
    namespace DaemonTask {
        class ITask {
            public:
                static bool handshake(nar::Socket& skt, nar::Global* globals);
                virtual void run(int unx_sockfd, nar::Global* globals) = 0;
        };
    }
}

#endif
