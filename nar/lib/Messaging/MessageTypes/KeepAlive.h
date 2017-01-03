#ifndef NAR_MESSKEEPALIVE_H
#define NAR_MESSKEEPALIVE_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "MessageRS.h"

namespace nar {
    namespace messagetypes {
        namespace KeepAlive {
            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("keepalive")) {}
                    void sendMessage();
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("keepalive")) {}
                    void sendMessage();
            };
        }
    }
}

#endif
