#ifndef NAR_MESSWAITCHUNKPUSH_H
#define NAR_MESSWAITCHUNKPUSH_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace WaitChunkPush {
            class Response : public ResponseHeader {
                public:
                    Response(int statcode) : ResponseHeader(statcode, std::string("wait_chunk_push_request")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json wpush_resp_recv);
                    nlohmann::json test_json();
            };
            class Request : public RequestHeader {
                private:
                    unsigned short rand_port;
                    long long int stream_id;
                    long long int chunk_id;
                    long long int chunk_size;
                public:
                    Request(): RequestHeader(std::string("wait_chunk_push_request")) {}
                    Request(unsigned short rport,long long int sid, long long int cid, long long int csize): RequestHeader(std::string("wait_chunk_push_request")), rand_port(rport), stream_id(sid), chunk_id(cid), chunk_size(csize){}
                    long long int get_stream_id();
                    unsigned short get_randevous_port();
                    long long int  get_chunk_id();
                    long long int  get_chunk_size();
                    void send_mess(nar::Socket* skt, nar::MessageTypes::WaitChunkPush::Response & resp);
                    void receive_message(nlohmann::json wpush_req_recv);
                    nlohmann::json test_json();

            };

        }
    }
}



#endif
