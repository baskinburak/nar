#ifndef NAR_MESS_ChunkReceiveRequest_H
#define NAR_MESS_ChunkReceiveRequest_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
namespace nar {
    namespace messagetypes {
        namespace ChunkReceiveRequest {
            class Request : public RequestHeader {
                private:
					std::string token;
                    long long int chunkId;
					unsigned long chunkSize;
                public:
                    Request(std::string _token, long long int _chunkId, unsigned long _chunkSize) : RequestHeader(std::string("chunk_receive_request")), token(_token),
																																		 chunkId(_chunkId), chunkSize(_chunkSize) {}
                    long long int get_chunkId();
					std::string get_token();
					unsigned long get_chunkSize();
            };

            class Response : public ResponseHeader {
                public:
                    Response(int statcode): ResponseHeader(statcode, std::string("chunk_receive_request"))  {}
            };
        }
    }
}

#endif
