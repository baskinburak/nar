#ifndef NAR_MESS_HANDSHAKE_H
#define NAR_MESS_HANDSHAKE_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace Handshake {
            class Request : public RequestHeader {
                private:
                    std::string username;
                public:
                    Request(std::string _username):RequestHeader(std::string("handshake")), username(_username) {}
                    std::string& get_username();
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode = 200): ResponseHeader(statcode, std::string("handshake")) {}
            };
        }
    }
}

#endif
