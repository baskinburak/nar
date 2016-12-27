#ifndef NAR_MESSPEERPORTREQUEST_H
#define NAR_MESSPEERPORTREQUEST_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"

namespace nar {
    namespace messagetypes {
        namespace PeerPortRequest {
            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("peer_port_request")){}


            };
            class Response : public ResponseHeader {
                private:
                    int port_number;
                public:
                    Response(int statcode,int pnum) : ResponseHeader(statcode, std::string("peer_port_request")),port_number(pnum) {}
                    int get_port_number();
            };
        }
    }
}
#endif
