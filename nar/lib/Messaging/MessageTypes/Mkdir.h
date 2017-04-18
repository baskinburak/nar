//
// Created by fatih on 18.04.2017.
//

#ifndef NARMESS_MKDIR_H
#define NARMESS_MKDIR_H

#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace Mkdir {
            class Response : public ResponseHeader {
            public:
                Response() :ResponseHeader(-1, std::string("mkdir")) {}
                Response(int statcode ) : ResponseHeader(statcode, std::string("mkdir")) {}
                void send_mess(nar::Socket* skt);
                void receive_message(nlohmann::json& keep_resp_recv);
                nlohmann::json test_json();
            };

            class Request : public RequestHeader {
            private:
                std::string _dest_dir;
                std::string _dir_name;
            public:
                Request(): RequestHeader(std::string("mkdir")) {}
                Request(std::string dir_name,std::string dest_dir): RequestHeader(std::string("mkdir")), _dir_name(dir_name), _dest_dir(dest_dir) {}
                void send_mess(nar::Socket* skt, nar::MessageTypes::Mkdir::Response & resp);
                void receive_message(std::string& msg);
                std::string& get_dest_dir();
                std::string& get_dir_name();
                nlohmann::json test_json();
            };
        }
    }
}


#endif //NARMESS_MKDIR_H
