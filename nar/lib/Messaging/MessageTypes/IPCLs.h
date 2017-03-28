#ifndef NAR_MESSIPCLS_H
#define NAR_MESSIPCLS_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
//#include "../../nlohJson/json.hpp"
//#include "../../../narnode/utility.h"
#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCLs {
            class Request : public IPCBaseRequest {
                private:
                    std::string dir_name;
                public:
                    std::string get_dir_name();
                    void set_dir_name(std::string dn);

                    Request(std::string dn) : IPCBaseRequest(std::string("ls")), dir_name(dn) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    //simdilik void send__action(nar::Socket* skt);
                    //void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("ls"), sc) {}
                    nlohmann::json give_myresponsejson();
                    //void send_message_progress(nar::Socket* skt, int progress);
                    //void send_message_end(nar::Socket* skt)
                    //void receive_message(nlohmann::json push_req_recv);
            };
        }
    }
}

#endif
