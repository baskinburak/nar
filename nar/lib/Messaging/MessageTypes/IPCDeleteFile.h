#ifndef NAR_MESSIPCDELFILE_H
#define NAR_MESSIPCDELFILE_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>


namespace nar {
    namespace MessageTypes {
        namespace IPCDeleteFile {
            class Request : public IPCBaseRequest {
                private:
                    std::string _file_name;
                    std::string _dest_dir;
                public:
                    std::string get_file_name();
                    std::string get_dest_dir();
                    void set_file_name(std::string dn);
                    void set_dest_dir(std::string dd);

                    Request() : IPCBaseRequest(std::string("delete_file")) {}
                    Request(std::string file_name, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("delete_file"), username, password, curdir), _file_name(file_name), _dest_dir(std::string("")) {}
                    Request(std::string dest_dir, std::string file_name, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("delete_file"), username, password, curdir), _file_name(file_name), _dest_dir(dest_dir) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);

                    nlohmann::json generate_json();
                    void populate_object(std::string& jsn);
					//char* masctime(const struct tm *timeptr);
					void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
				private:

                public:

					Response() : IPCBaseResponse(100, std::string("delete_file"), 200) {}

                    Response( long int prog, long int sc) : IPCBaseResponse(prog, std::string("delete_file"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
