#ifndef NAR_MESSIPCPUSH_H
#define NAR_MESSIPCPUSH_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
//#include "../../nlohJson/json.hpp"
//#include "../../../narnode/utility.h"
#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCPush {
            class Request : public IPCBaseRequest {
                private:
                    std::string file_name;
                public:
                    std::string get_file_name();
                    void set_file_name(std::string fn);

                    Request(std::string fn) : IPCBaseRequest(std::string("push")), file_name(fn) {}
                    nlohmann::json get_myrequestjson();
                    //simdilik void send_action(nar::Socket* skt);
                    //void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("push"), sc) {}
                    nlohmann::json give_myresponsejson();
                    //void send_message_progress(nar::Socket* skt, int progress);
                    //void send_message_end(nar::Socket* skt)
                    //void receive_message(nlohmann::json push_req_recv);
            };
        }
    }
}

#endif
