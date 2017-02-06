#ifndef NAR_MESS_HANDSHAKE_H
#define NAR_MESS_HANDSHAKE_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace Handshake {
            class Request : public RequestHeader {
                private:
                    std::string username;
                    std::string machine_id;
                public:
                    Request(std::string _username,std::string _machine_id):RequestHeader(std::string("handshake")), username(_username) , machine_id(_machine_id) {}
                    std::string& getUsername();
                    std::string& getMachineId();
                    void sendMessage();
                    void receiveMessage(nlohmann::json hand_req_recv);
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode = 200): ResponseHeader(statcode, std::string("handshake")) {}
                    void sendMessage();
                    void receiveMessage(nlohmann::json hand_resp_recv);
            };
        }
    }
}

#endif
