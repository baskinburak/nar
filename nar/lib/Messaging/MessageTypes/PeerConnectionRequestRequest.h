#ifndef NAR_MESSPEERCONNECTIONREQUESTREQUEST_H
#define NAR_MESSPEERCONNECTIONREQUESTREQUEST_H

#include <string>
#include "Request.h"

namespace nar {
    namespace messagetypes {
        class PeerConnectionRequestRequest : public Request {
            private:
                std::string peerid;
            public:
                PeerConnectionRequestRequest(std::string p): Request(std::string("peer_connection_request")), peerid(p) {}
                std::string& get_peerid();
        };
    }
}

#endif
