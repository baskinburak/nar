#ifndef NAR_MESSIPCLS_H
#define NAR_MESSIPCLS_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>
#include <vector>

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
					//char* masctime(const struct tm *timeptr);
					void print_loop(nar::Socket* skt);
            };

            class Response : public IPCBaseResponse {
				private:
					std::string _entity_name;
					std::string _entity_size;
					std::string _change_time;
					std::string _type;
                public:
					std::string get_entity_name();
					std::string get_entity_size();
					std::string get_change_time();
					std::string get_type();
					void send_message_progress(nar::Socket* skt, int p);
					void set_entity_name(std::string en);
					void set_entity_size(std::string es);
					void set_change_time(std::string ct);
					void set_type(std::string t);
					Response() : IPCBaseResponse(100, std::string("ls"), 200), _entity_name(std::string("")), _entity_size(std::string("")), _change_time(std::string("")), _type(std::string("")) {}
					Response(int stat):IPCBaseResponse(0, std::string("ls"), stat) ,_entity_name(std::string("")), _entity_size(std::string("")), _change_time(std::string("")), _type(std::string("")) {}
					Response(int prog ,int sc):IPCBaseResponse(prog, std::string("ls"), sc) ,_entity_name(std::string("")), _entity_size(std::string("")), _change_time(std::string("")), _type(std::string("")) {}

                    Response(std::string en, std::string es, std::string ct, std::string t, long int prog, long int sc) : IPCBaseResponse(prog, std::string("ls"), sc), _entity_name(en), _entity_size(es), _change_time(ct), _type(t) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
