#ifndef NAR_MESSREGISTER_H
#define NAR_MESSREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "MessageRS.h"

namespace nar {
    namespace messagetypes {
        namespace Register {
            class Request : public RequestHeader {
                private:
                    std::string username;
                    std::string aes;
                public:
                    Request(std::string uname, std::string a): RequestHeader(std::string("register")), username(uname), aes(a){}
                    std::string& get_username();
                    std::string& get_aes();
                    void sendMessage();

            };
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("register")) {}
                    void sendMessage();
            };
        }
    }
}


#endif
