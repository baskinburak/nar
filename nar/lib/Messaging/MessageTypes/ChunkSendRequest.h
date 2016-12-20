#ifndef NAR_MESSCHUNKSENDREQUEST_H
#define NAR_MESSCHUNKSENDREQUEST_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"

namespace nar {
    namespace messagetypes {
        namespace ChunkSendRequest {
            class Request : public RequestHeader {
                private:
                    std::string token;
                    long long int chunk_id;
                    long long int chunk_size;
                public:
                    Request(std::string t, long long int cid, long long int csize): RequestHeader(std::string("chunk_send_request")), token(t), chunk_id(cid), chunk_size(csize){}
                    std::string& get_token();
                    long long int  get_chunk_id();
                    long long int  get_chunk_size();

            };
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("chunk_send_request")) {}
            };
        }
    }
}
#endif
