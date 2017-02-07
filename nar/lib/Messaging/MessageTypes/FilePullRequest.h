#ifndef NAR_MESSFILEPULLREQUESTREQUEST_H
#define NAR_MESSFILEPULLREQUESTREQUEST_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace FilePullRequest {
            class Request : public RequestHeader {
                private:
                    std::string dir;
                    std::string file_name;
                public:
                    Request(std::string fn, std::string d): RequestHeader(std::string("file_pull_request")),  dir(d), file_name(fn) {}
                    std::string& get_filename();
                    std::string& get_dir();
                    void sendMessage();
                    void receiveMessage(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
            };

            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string machine_id;
                        std::string chunk_id;
                        std::string stream_id;
                        unsigned long long int chunk_size;
                    };
                    Response(int statcode, unsigned short  rport): ResponseHeader(statcode, std::string("file_pull_request")), randevous_port(rport) {}
                    Response(int statcode, unsigned short  rport,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_pull_request")), randevous_port(rport),  elements(eles) {}
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string mid, std::string cid, std::string sid, unsigned long long int csize);
                    std::vector<struct PeerListElement>& get_elements();
                    unsigned short get_randevous_port();
                    void sendMessage();
                    void receiveMessage(nlohmann::json pull_resp_recv);
                private:
                    unsigned short randevous_port;
                    std::vector<struct PeerListElement> elements;
                    nlohmann::json test_json();

            };
        }
    }
}

#endif
