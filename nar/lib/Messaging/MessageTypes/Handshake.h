#ifndef NAR_MESS_HANDSHAKE_H
#define NAR_MESS_HANDSHAKE_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>
namespace nar {
    namespace Messagetypes {
        namespace Handshake {
            class Request : public RequestHeader {
                private:
                    std::string username;
                    std::string machine_id;
                public:
                    Request(std::string _username,std::string _machine_id):RequestHeader(std::string("handshake")), username(_username) , machine_id(_machine_id) {}
                    std::string& get_username();
                    std::string& get_machine_id();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json hand_req_recv);
                    nlohmann::json test_json();
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode = 200): ResponseHeader(statcode, std::string("handshake")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json hand_resp_recv);
                    nlohmann::json test_json();
            };
        }
    }
}

#endif
