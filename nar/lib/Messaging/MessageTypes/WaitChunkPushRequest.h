#ifndef NAR_MESSWAITCHUNKPUSHREQUEST_H
#define NAR_MESSWAITCHUNKPUSHREQUEST_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"
namespace nar {
    namespace messagetypes {
        namespace WaitChunkPushRequest {
            class Request : public RequestHeader {
                private:
                    unsigned short rand_port;
                    std::string stream_id;
                    long long int chunk_id;
                    long long int chunk_size;
                public:
                    Request(unsigned short rport,std::string sid, long long int cid, long long int csize): RequestHeader(std::string("wait_chunk_push_request")), rand_port(rport), stream_id(sid), chunk_id(cid), chunk_size(csize){}
                    std::string& get_stream_id();
                    unsigned short get_randevous_port();
                    long long int  get_chunk_id();
                    long long int  get_chunk_size();
                    void sendMessage();
                    void receiveMessage(nlohmann::json wpush_req_recv);

            };
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("wait_chunk_push_request")) {}
                    void sendMessage();
                    void receiveMessage(nlohmann::json wpush_resp_recv);
            };
        }
    }
}



#endif
