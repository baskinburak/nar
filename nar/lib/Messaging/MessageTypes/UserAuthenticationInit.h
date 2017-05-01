#ifndef NAR_MESSUSERAUTHINIT_H
#define NAR_MESSUSERAUTHINIT_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <nar/lib/Exception/Exception.h>
namespace nar {
    namespace MessageTypes {
        namespace UserAuthenticationInit {
            class Response : public ResponseHeader {
                private:
                    std::string _private_key;
                    std::string _task_string;
                    std::string _aes_crypted;
                public:
                    Response();
                    Response(int statcode);
                    Response(int statcode,std::string aes_crypted, std::string private_key, std::string task_string) : ResponseHeader(statcode, std::string("user_authentication_init")), _private_key(private_key), _task_string(task_string), _aes_crypted(aes_crypted) {}
                    std::string& get_private_key();
                    std::string& get_task_string();
                    std::string& get_aes_crypted();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json keep_resp_recv);
                    nlohmann::json test_json();
            };



            class Request : public RequestHeader {
                private:
                    std::string _username;
                public:
                    Request() : RequestHeader(std::string("user_authentication_init")) {}
                    Request(std::string username ): RequestHeader(std::string("user_authentication_init")), _username(username) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::UserAuthenticationInit::Response & resp);
                    void receive_message(std::string& recv_msg);
                    nlohmann::json test_json();
                    std::string& get_username();
            };


        }
    }
}





#endif
