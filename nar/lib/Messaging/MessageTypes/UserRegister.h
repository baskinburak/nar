#ifndef NAR_MESSUSERREGISTER_H
#define NAR_MESSUSERREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace Messagetypes {
        namespace UserRegister {
            class Request : public RequestHeader {
                private:
                    std::string username;
                    std::string aes;
                public:
                    Request(std::string uname, std::string a): RequestHeader(std::string("user_register")), username(uname), aes(a){}
                    std::string& get_username();
                    std::string& get_aes();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json reg_req_recv);
                    nlohmann::json test_json();

            };
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("user_register")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json reg_resp_recv);
                    nlohmann::json test_json();
            };
        }
    }
}


#endif
