#ifndef NAR_MESSIPCPUSH_H
#define NAR_MESSIPCPUSH_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCPush {
            class Request : public IPCBaseRequest {
                private:
                    std::string _file_name;
                public:
                    std::string get_file_name();
                    void set_file_name(std::string fn);

                    Request(std::string fn) : IPCBaseRequest(std::string("push")), _file_name(fn) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("push"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
