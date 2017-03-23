#ifndef NAR_MESSIPCREGISTER_H
#define NAR_MESSIPCREGISTER_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCRegister {
            class Request : public IPCBaseRequest {
                private:
                    std::string user_name;
                public:
                    Request(std::string un) : IPCBaseRequest(std::string("register")), user_name(un) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);
                    //void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("register"), sc) {}
                    nlohmann::json give_myresponsejson();
                    //void send_message_progress(nar::Socket* skt, int progress);
                    //void send_message_end(nar::Socket* skt)
                    //void receive_message(nlohmann::json push_req_recv);
            };
        }
    }
}

#endif
