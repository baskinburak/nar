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
            /*
                Stat Codes for KeepAlive
                200 -> Success
                300 -> Machine ID not valid
                301 -> Message Not Valid [HEADER or PAYLOAD problems]
            */
            class Response : public ResponseHeader {
                public:
                    Response() :ResponseHeader(-1, std::string("keepalive")) {}
                    Response(int statcode ) : ResponseHeader(statcode, std::string("keepalive")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json& keep_resp_recv);
                    nlohmann::json test_json();
            };

            class Request : public RequestHeader {
                private:
                    std::string _machine_id;
                public:
                    Request(): RequestHeader(std::string("keepalive")) {}
                    Request(std::string machine_id): RequestHeader(std::string("keepalive")), _machine_id(machine_id) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::KeepAlive::Response & resp);
                    void receive_message(std::string& msg);
                    std::string& get_machine_id();
                    nlohmann::json test_json();
            };
        }
    }
}

#endif
