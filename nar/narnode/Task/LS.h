#ifndef NAR_LS_H
#define NAR_LS_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/nlohJson/json.hpp>
#include <nar/lib/Socket/Socket.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>

namespace nar {
    namespace task {
        class LS : public nar::task::ITask {
            private:
                std::string _dir;
                void createReqJson(nlohmann::json &js,std::string & user_name);
                void getResJson(nlohmann::json &js,nar::Socket* con_socket);
                Socket * createServerConnection(nar::Global* globals);
                bool sendReqJson(nlohmann::json &jreq, nar::Socket* con_socket);
                char* masctime(const struct tm *timeptr);
            public:
                LS(std::string dir):_dir(dir) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}

#endif  //NAR_LS_H
