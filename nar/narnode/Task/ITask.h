#ifndef NAR_ITASK_H
#define NAR_ITASK_H
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>

namespace nar {
    namespace task {
        class ITask {
            private:
                bool keepalive;
                nar::Socket keepalive_sckt;
            public:
                bool handshake(nar::Socket& skt, nar::Global* globals);
                virtual void run(int unx_sockfd, nar::Global* globals) = 0;
                void keepalive(nar::Global* globals);
        };
    }
}

#endif
