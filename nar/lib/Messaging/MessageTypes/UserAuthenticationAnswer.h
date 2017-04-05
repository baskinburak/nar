#ifndef NAR_MESSUSERAUTHANS_H
#define NAR_MESSUSERAUTHANS_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <nar/lib/Exception/Exception.h>
namespace nar {
    namespace MessageTypes {
        namespace UserAuthenticationAnswer {
            class Response : public ResponseHeader {
                public:
                    Response() :ResponseHeader(-1, std::string("user_authentication_answer")) {}
                    Response(int statcode) : ResponseHeader(statcode, std::string("user_authentication_answer")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json uans_resp_recv);
                    nlohmann::json test_json();
            };



            class Request : public RequestHeader {
                private:
                    std::string _result_string;
                public:
                    Request() : RequestHeader(std::string("user_authentication_answer")){}
                    Request(std::string result_string ): RequestHeader(std::string("user_authentication_answer")), _result_string(result_string) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::UserAuthenticationAnswer::Response & resp);
                    void receive_message(std::string& recv_req);
                    nlohmann::json test_json();
                    std::string& get_result_string();
            };


        }
    }
}





#endif
