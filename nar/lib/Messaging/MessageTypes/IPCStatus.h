#ifndef NAR_MESSIPCSTATUS_H
#define NAR_MESSIPCSTATUS_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCStatus {
            class Request : public IPCBaseRequest {
                public:
                    Request() : IPCBaseRequest(std::string("status")) {}
					Request(std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("status"), username, password, curdir) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);

					nlohmann::json generate_json();
					void populate_object(std::string& jsn_str);
                    void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
                private:
                    std::string _nar_folder;
                    std::string _file_folder;
                    std::string _server_ip;
                    std::string _server_port;
                    std::string _machine_id;
                public:
                    std::string get_nar_folder();
                    std::string get_file_folder();
                    std::string get_server_ip();
                    std::string get_server_port();
                    std::string get_machine_id();
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("status"), sc) {}
                    Response(long int prog, long int sc, std::string nar_folder, std::string file_folder,
                    std::string server_ip, std::string server_port, std::string machine_id)
                    : IPCBaseResponse(prog, std::string("status"), sc),
                     _nar_folder(nar_folder), _file_folder(file_folder) , _server_ip(server_ip),
                     _server_port(server_port), _machine_id(machine_id) {}
                    nlohmann::json give_myresponsejson();
                    void send_message(nar::Socket* skt);
            };
        }
    }
}

#endif
