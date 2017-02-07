#ifndef NAR_MESSFILEPUSH_H
#define NAR_MESSFILEPUSH_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>
namespace nar {
    namespace Messagetypes {
        namespace FilePush {
            class Request : public RequestHeader {
                private:
                    unsigned long long int filesize;
                    std::string dir;
                    std::string filename;
                public:
                    Request(std::string fn, std::string d, unsigned long long int fs): RequestHeader(std::string("file_push_request")), filesize(fs), dir(d), filename(fn) {}
                    std::string& get_filename();
                    std::string& get_dir();
                    unsigned long long int get_filesize();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json push_req_recv);
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
                    Response(int statcode, unsigned short  rport): ResponseHeader(statcode, std::string("file_push_request")), randevous_port(rport) {}
                    Response(int statcode, unsigned short  rport,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_push_request")), randevous_port(rport),  elements(eles) {}
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string mid, unsigned long long int cid, std::string sid, unsigned long long int csize);
                    std::vector<struct PeerListElement>& get_elements();
                    unsigned short get_randevous_port();
                    void send_mess(nar::Socket* skt);
                    nlohmann::json test_json();
                    /**
                      * A function that receives response message of the File Push Request
                      *
                      * @author = Fatih Can Kurnaz
                      * @param = push_resp_recv, nlohmann::json,  takes the received nlohmann::json object
                      * @todo = should act differently based on the received status_code
                      * @tested = No
                      * @return = void
                    */
                    void receive_message(nlohmann::json push_resp_recv);
                private:
                    unsigned short randevous_port;
                    std::vector<struct PeerListElement> elements;

            };
        }
    }
}

#endif
