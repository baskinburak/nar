#ifndef NAR_MESSIPCCONFIG_H
#define NAR_MESSIPCCONFIG_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
//#include "../../nlohJson/json.hpp"
//#include "../../../narnode/utility.h"
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

                    //simdilik void send_action(nar::Socket* skt);

                    //void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("config"), sc) {}
                    nlohmann::json give_myresponsejson();
                    //void send_message_progress(nar::Socket* skt, int progress);
                    //void send_message_end(nar::Socket* skt)
            };
        }
    }
}

#endif
