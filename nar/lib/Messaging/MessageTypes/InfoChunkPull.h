#ifndef NAR_MESS_InfoChunkPull_H
#define NAR_MESS_InfoCHunkPull_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace InfoChunkPull {
            class Response : public ResponseHeader {
                private:
                    long long int _stream_id;
                public:
                    Response():ResponseHeader(-1, std::string("info_chunk_pull")), _stream_id(0)  {}
                    Response( long long int stream_id,int statcode): ResponseHeader(statcode, std::string("info_chunk_pull")),  _stream_id(stream_id)  {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json ipull_resp_recv);
                    nlohmann::json test_json();
                    unsigned  short  get_r_port();
                    long long int get_stream_id();
            };

            class Request : public RequestHeader {
                private:
                    long long int chunk_id;
					bool success;
                public:
                    Request(long long int _chunkId  = -1, bool _success = false) : RequestHeader(std::string("info_chunk_pull")), chunk_id(_chunkId), success(_success) {}
                    long long int get_chunk_id();
					bool get_success();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json ipull_req_recv);
                    nlohmann::json test_json();
            };


        }
    }
}

#endif
