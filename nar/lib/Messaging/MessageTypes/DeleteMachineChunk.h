//
// Created by fatih on 2.05.2017.
//

#ifndef NARMESS_DELMACHINECHUNK_H
#define NARMESS_DELMACHINECHUNK_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace DeleteMachineChunk {
            class Response : public ResponseHeader {
            public:
                Response() :ResponseHeader(-1, std::string("delete_machine_chunk")) {}
                Response(int statcode ) : ResponseHeader(statcode, std::string("delete_machine_chunk")) {}
                void send_mess(nar::Socket* skt);
                void receive_message(nlohmann::json& keep_resp_recv);
                nlohmann::json test_json();
            };

            class Request : public RequestHeader {
            private:
                std::string _chunk_id;
            public:
                Request(): RequestHeader(std::string("delete_machine_chunk")) {}
                Request(std::string chunk_id): RequestHeader(std::string("delete_machine_chunk")), _chunk_id(chunk_id) {}
                void send_mess(nar::Socket* skt, nar::MessageTypes::DeleteMachineChunk::Response & resp);
                void receive_message(std::string& msg);
                std::string& get_chunk_id();
                nlohmann::json test_json();
            };
        }
    }
}


#endif //NARMESS_DELMACHINECHUNK_H
