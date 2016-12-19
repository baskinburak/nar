#ifndef NAR_MESS_PEERPORTPUSHREQ_H
#define NAR_MESS_PEERPORTPUSHREQ_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace PeerPortPushRequest {
            class Request : public RequestHeader {
                private:
                    std::string token;
                public:
                    Request(std::string _token):RequestHeader(std::string("peer_port_push_request")), token(_token) {}
                    std::string& get_token();
            };

            class Response : public ResponseHeader {
				private:
					int port;
                public:
                    Response(int statcode, int portNum): ResponseHeader(statcode, std::string("peer_port_push_request")), port(portNum) {}
					int get_port();
            };
        }
    }
}

#endif
