#ifndef NAR_MESSKEEPALIVE_H
#define NAR_MESSKEEPALIVE_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"

namespace nar {
    namespace messagetypes {
        namespace KeepAlive {
            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("keepalive")) {}
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("keepalive")) {}
            };
        }
    }
}

#endif
