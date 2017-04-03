#ifndef NAR_MESSIPCLS_H
#define NAR_MESSIPCLS_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCLs {
            class Request : public IPCBaseRequest {
                private:
                    std::string _dir_name;
                public:
                    std::string get_dir_name();
                    void set_dir_name(std::string dn);

                    Request(std::string dn) : IPCBaseRequest(std::string("ls")), _dir_name(dn) {}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);
                    /*
                     * get a message
                     *
                     * @author: Dogu
                     * @param:  None
                     * @tested: No
                    */
                    void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("ls"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
