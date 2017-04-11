#ifndef NAR_MESSKEEPALIVECHECK_H
#define NAR_MESSKEEPALIVECHECK_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace KeepAliveCheck {
            class Response : public ResponseHeader {
                public:
                    Response() :ResponseHeader(-1, std::string("keepalive_check")) {}
                    Response(int statcode ) : ResponseHeader(statcode, std::string("keepalive_check")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json& keep_resp_recv);
                    nlohmann::json test_json();
            };

            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("keepalive_check")) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::KeepAliveCheck::Response & resp);
                    void receive_message(std::string& msg);
                    nlohmann::json test_json();
            };
        }
    }
}

#endif
