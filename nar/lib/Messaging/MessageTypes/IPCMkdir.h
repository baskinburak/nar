#ifndef NAR_MESSIPCMKDIR_H
#define NAR_MESSIPCMKDIR_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>
#include <vector>


namespace nar {
    namespace MessageTypes {
        namespace IPCMkdir {
            class Request : public IPCBaseRequest {
                private:
                    std::string _dir_name;
                    std::string _dest_dir;
                public:
                    std::string get_dir_name();
                    std::string get_dest_dir();
                    void set_dir_name(std::string dn);
                    void set_dest_dir(std::string dd);
                    Request() : IPCBaseRequest(std::string("mkdir")) {}
                    Request(std::string dir_name, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("mkdir"), username, password, curdir), _dir_name(dir_name), _dest_dir(std::string("")) {}
                    Request(std::string dir_name, std::string dest_dir, std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("mkdir"), username, password, curdir), _dir_name(dir_name), _dest_dir(dest_dir) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);
                    /*
                     * returns the json form of base request
                    */
                    nlohmann::json generate_json();
                    void populate_object(std::string& jsn);
					//char* masctime(const struct tm *timeptr);
					void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
				private:

                public:

					Response() : IPCBaseResponse(100, std::string("ls"), 200) {}

                    Response( long int prog, long int sc) : IPCBaseResponse(prog, std::string("mkdir"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
