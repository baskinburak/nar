#ifndef NAR_LS_H
#define NAR_LS_H

#include <nar/narnode/Task/ITask.h>
#include <iostream>
#include <nar/narnode/global.h>
namespace nar {
    namespace task {
        class LS : public nar::task::ITask {
            public:
                LS() { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}

#endif
