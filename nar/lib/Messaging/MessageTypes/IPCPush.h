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
                    std::string _file_path;
                public:
                    std::string get_file_path();
                    void set_file_path(std::string fp);

					Request() : IPCBaseRequest(std::string("push")) {}
                    Request(std::string fp, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("push"), username, password, curdir), _file_path(fp) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);

					nlohmann::json generate_json();
					void populate_object(std::string& jsn_str);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response() : IPCBaseResponse(100, std::string("push"), -1) {}
                    Response(long int sc) : IPCBaseResponse(100, std::string("push"), sc) {}
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("push"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
