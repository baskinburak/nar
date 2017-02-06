#ifndef NAR_MESSUSERREGISTER_H
#define NAR_MESSUSERREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"
namespace nar {
    namespace messagetypes {
        namespace UserRegister {
            class Request : public RequestHeader {
                private:
                    std::string username;
                    std::string aes;
                public:
                    Request(std::string uname, std::string a): RequestHeader(std::string("user_register")), username(uname), aes(a){}
                    std::string& get_username();
                    std::string& get_aes();
                    void sendMessage();
                    void receiveMessage(nlohmann::json reg_req_recv);

            };
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("user_register")) {}
                    void sendMessage();
                    void receiveMessage(nlohmann::json reg_resp_recv);
            };
        }
    }
}


#endif
