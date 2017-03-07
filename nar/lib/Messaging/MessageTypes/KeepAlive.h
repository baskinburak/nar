#ifndef NAR_MESSKEEPALIVE_H
#define NAR_MESSKEEPALIVE_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace KeepAlive {
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("keepalive")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json keep_resp_recv);
                    nlohmann::json test_json();
            };

            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("keepalive")) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::KeepAlive::Response & resp);
                    void receive_message(nlohmann::json keep_req_recv);
                    nlohmann::json test_json();
            };
        }
    }
}

#endif
