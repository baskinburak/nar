#ifndef NAR_MESSFILEPULL_H
#define NAR_MESSFILEPULL_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>
namespace nar {
    namespace Messagetypes {
        namespace FilePull {
            class Request : public RequestHeader {
                private:
                    std::string dir;
                    std::string file_name;

                public:
                    Request(std::string fn, std::string d): RequestHeader(std::string("file_pull_request")),  dir(d), file_name(fn) {}
                    std::string& get_filename();
                    std::string& get_dir();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
            };

            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string machine_id;
                        unsigned long long int chunk_id;
                        std::string stream_id;
                        unsigned long long int chunk_size;
                    };
                    Response(int statcode, unsigned short  rport): ResponseHeader(statcode, std::string("file_pull_request")), randevous_port(rport) {}
                    Response(int statcode, unsigned short  rport,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_pull_request")), randevous_port(rport),  elements(eles) {}
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string mid, unsigned long long int cid, std::string sid, unsigned long long int csize);
                    std::vector<struct PeerListElement>& get_elements();
                    unsigned short get_randevous_port();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
                private:
                    unsigned short randevous_port;
                    std::vector<struct PeerListElement> elements;



            };
        }
    }
}

#endif
