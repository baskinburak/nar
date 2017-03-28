#ifndef NAR_DAEMON_TASK_LS_H
#define NAR_DAEMON_TASK_LS_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>

namespace nar {
    namespace DaemonTask {
        class LS : public nar::task::ITask {
            private:
                std::string _dir;
                char* masctime(const struct tm *timeptr);
            public:
                LS(std::string dir):_dir(dir) { }
                void run(nar::Socket * skt, nar::Global* globals);
        };
    }
}

#endif  //NAR_LS_H
