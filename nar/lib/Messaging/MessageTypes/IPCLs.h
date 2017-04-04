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

                    Request() : IPCBaseRequest(std::string("ls")) {}
                    Request(std::string dirname, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("ls"), username, password, curdir), _dir_name(dirname) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);
                    /*
                     * returns the json form of base request
                    */
                    nlohmann::json generate_json();
                    void populate_object(std::string& jsn);
            };

            class Response : public IPCBaseResponse {
                private:
                    item_name
                    change_time
                    item_size
                    item_type
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("ls"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
