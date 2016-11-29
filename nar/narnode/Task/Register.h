#ifndef NAR_REGISTER_H
#define NAR_REGISTER_H

#include <nar/narnode/Task/ITask.h>
#include <iostream>
#include <nar/narnode/global.h>
namespace nar {
    namespace task {
        class Register : public nar::task::ITask {
            private:
                std::string username;
            public:
                Register(std::string uname):username(uname) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}

#endif  //NAR_LS_H
