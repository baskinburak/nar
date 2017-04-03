#ifndef NAR_DAEMON_TASK_REGISTER_H
#define NAR_DAEMON_TASK_REGISTER_H

#include <nar/narnode/Task/ITask.h>
#include <iostream>
#include <nar/narnode/global.h>
namespace nar {
    namespace DaemonTask {
        class Register : public nar::DaemonTask::ITask {
            private:
                std::string user_name;
            public:
                Register(std::string uname):user_name(uname) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}

#endif  //NAR_LS_H
