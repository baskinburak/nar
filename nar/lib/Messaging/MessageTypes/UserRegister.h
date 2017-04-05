#ifndef NAR_MESSUSERREGISTER_H
#define NAR_MESSUSERREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace UserRegister {
            class Response : public ResponseHeader {
                public:
                    Response(int statcode = -1) : ResponseHeader(statcode, std::string("user_register")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json reg_resp_recv);
                    nlohmann::json test_json();
            };
            class Request : public RequestHeader {
                private:
                    std::string _username;
                    std::string _aes_crypted;
                    std::string _rsa_pub;
                    std::string _rsa_pri_crypted;
                public:
                    Request(): RequestHeader(std::string("user_register")) {}
                    Request(std::string username, std::string aes_crypted, std::string rsa_pub, std::string rsa_pri_crypted): RequestHeader(std::string("user_register")), _username(username), _aes_crypted(aes_crypted), _rsa_pub(rsa_pub), _rsa_pri_crypted(rsa_pri_crypted) {}
                    std::string& get_username();
                    std::string& get_aes_crypted();
                    std::string& get_rsa_pub();
                    std::string& get_rsa_pri_crypted();
                    void send_mess(nar::Socket* skt, nar::MessageTypes::UserRegister::Response & resp);
                    void receive_message(std::string& reg_req_recv);
                    nlohmann::json test_json();

            };

        }
    }
}


#endif
