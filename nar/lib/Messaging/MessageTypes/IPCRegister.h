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
                private:
                    std::string _user_name;
                public:
                    std::string get_user_name();
                    void set_user_name(std::string un);

                    Request(std::string un) : IPCBaseRequest(std::string("register")), _user_name(un) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);
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
