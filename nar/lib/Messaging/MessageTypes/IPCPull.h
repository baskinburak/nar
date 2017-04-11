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
                    std::string _dir_name;
                public:
                    std::string get_file_name();
                    void set_file_name(std::string fn);

					Request() : IPCBaseRequest(std::string("pull")) {}
                    Request(std::string fn,std::string dn, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("pull"), username, password, curdir), _file_name(fn) ,_dir_name(dn){}
                    nlohmann::json get_myrequestjson();
                    void receive_message(nlohmann::json &js);
                    void send_action(nar::Socket* skt);
                    std::string get_dir_path();
					nlohmann::json generate_json();
					void populate_object(std::string& jsn_str);
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
