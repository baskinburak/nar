//
// Created by fatih on 18.04.2017.
//

#ifndef NARMESS_DELFILE_H
#define NARMESS_DELFILE_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace DeleteFile {
            class Response : public ResponseHeader {
            public:
                Response() :ResponseHeader(-1, std::string("delete_file")) {}
                Response(int statcode ) : ResponseHeader(statcode, std::string("delete_file")) {}
                void send_mess(nar::Socket* skt);
                void receive_message(nlohmann::json& keep_resp_recv);
                nlohmann::json test_json();
            };

            class Request : public RequestHeader {
            private:
                std::string _dest_dir;
                std::string _file_name;
            public:
                Request(): RequestHeader(std::string("delete_file")) {}
                Request(std::string file_name,std::string dest_dir): RequestHeader(std::string("delete_file")), _file_name(file_name), _dest_dir(dest_dir) {}
                void send_mess(nar::Socket* skt, nar::MessageTypes::DeleteFile::Response & resp);
                void receive_message(std::string& msg);
                std::string& get_dest_dir();
                std::string& get_file_name();
                nlohmann::json test_json();
            };
        }
    }
}


#endif //NARMESS_MKDIR_H
