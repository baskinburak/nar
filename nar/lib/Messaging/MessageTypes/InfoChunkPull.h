#ifndef NAR_MESS_InfoChunkPull_H
#define NAR_MESS_InfoCHunkPull_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
namespace nar {
    namespace messagetypes {
        namespace InfoChunkPull {
            class Request : public RequestHeader {
                private:
                    long long int chunkId;
					bool success;
                public:
                    Request(long long int _chunkId, bool _success) : RequestHeader(std::string("info_chunk_pull")), chunkId(_chunkId), success(_success) {}
                    long long int get_chunkId();
					bool get_success();
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode): ResponseHeader(statcode, std::string("info_chunk_pull"))  {}
            };
        }
    }
}

#endif
