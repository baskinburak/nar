#ifndef NAR_MESSIPCPULL_H
#define NAR_MESSIPCPULL_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
//#include "../../nlohJson/json.hpp"
//#include "../../../narnode/utility.h"
#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCPull {
            class Request : public IPCBaseRequest {
                private:
                    std::string file_name;
                    std::string cur_dir;
                public:
                    std::string get_file_name();
                    std::string get_cur_dir();
                    void set_file_name(std::string fn);
                    void set_cur_dir(std::string cd);

                    Request(std::string fn, std::string cd) : IPCBaseRequest(std::string("pull")), file_name(fn), cur_dir(cd) {}
                    nlohmann::json get_myrequestjson();
                    //simdilik void send_action(nar::Socket* skt);
                    //void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("pull"), sc) {}
                    nlohmann::json give_myresponsejson();
                    //void send_message_progress(nar::Socket* skt, int progress);
                    //void send_message_end(nar::Socket* skt)
                    //void receive_message(nlohmann::json push_req_recv);
            };
        }
    }
}

#endif
