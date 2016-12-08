#ifndef NAR_MESSWAITCHUNKPUSHREQUEST_H
#define NAR_MESSWAITCHUNKPUSHREQUEST_H

#include "Request.h"
#include <string>

namespace nar {
    namespace messagetypes {
        class WaitChunkPushRequest : public Request {
            private:
                std::string token;
                std::string chunkid;
                unsigned long long int chunksize;
            public:
                WaitChunkPushRequest(std::string t, std::string c, unsigned long long int cs): Request(std::string("wait_chunk_push_request")), token(t), chunkid(c), chunksize(cs) {}
                std::string& get_token();
                std::string& get_chunkid();
                unsigned long long int get_chunksize();
        };
    }
}

#endif
