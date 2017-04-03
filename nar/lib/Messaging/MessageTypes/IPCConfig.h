#ifndef NAR_MESSIPCCONFIG_H
#define NAR_MESSIPCCONFIG_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCConfig {
            class Request : public IPCBaseRequest {
                private:
                    std::string _var;
                    std::string _value;
                public:
                    Request(std::string var, std::string value) : IPCBaseRequest(std::string("config")), _var(var), _value(value) {}

                    std::string get_var();
                    std::string get_value();

                    void set_var(std::string var);
                    void set_value(std::string value);

                    nlohmann::json get_myrequestjson();

                    void receive_message(std::string &s);

                    void send_action(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("config"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
