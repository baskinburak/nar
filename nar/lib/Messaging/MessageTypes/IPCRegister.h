#ifndef NAR_MESSIPCREGISTER_H
#define NAR_MESSIPCREGISTER_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCRegister {
            class Request : public IPCBaseRequest {
                public:

					Request() : IPCBaseRequest(std::string("register")) {}
                    Request(std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("register"), username, password, curdir) {}
					
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);

					nlohmann::json generate_json();
					void populate_object(std::string& jsn_str);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("register"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
