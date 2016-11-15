#ifndef NAR_ITASK_H
#define NAR_ITASK_H
#include <nar/narnode/global.h>

namespace nar {
    namespace task {
        class ITask {
            public:
                virtual void run(int unx_sockfd, nar::Global* globals) = 0;
        };
    }
}

#endif
