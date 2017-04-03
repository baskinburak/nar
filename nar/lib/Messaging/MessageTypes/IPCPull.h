#ifndef NAR_MESSIPCPULL_H
#define NAR_MESSIPCPULL_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCPull {
            class Request : public IPCBaseRequest {
                private:
                    std::string _file_name;
                    std::string _cur_dir;
                public:
                    std::string get_file_name();
                    std::string get_cur_dir();
                    void set_file_name(std::string fn);
                    void set_cur_dir(std::string cd);

                    Request(std::string fn, std::string cd) : IPCBaseRequest(std::string("pull")), _file_name(fn), _cur_dir(cd) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("pull"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
