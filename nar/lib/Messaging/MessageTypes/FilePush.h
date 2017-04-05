#ifndef NAR_MESSFILEPUSH_H
#define NAR_MESSFILEPUSH_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>
namespace nar {
    namespace MessageTypes {
        namespace FilePush {
            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string machine_id;
                        unsigned long long int chunk_id;
                        unsigned int stream_id;
                        unsigned long long int chunk_size;
                        bool operator<(struct PeerListElement& rhs) { return chunk_id < rhs.chunk_id; }
                    };
                    Response() : ResponseHeader(-1, std::string("file_push_request")) {}
                    Response(int statcode , unsigned short  rport): ResponseHeader(statcode, std::string("file_push_request")), _randezvous_port(rport) {}
                    Response(int statcode , unsigned short  rport,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_push_request")), _randezvous_port(rport),  _elements(eles) {}
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string mid, unsigned long long int cid, unsigned int sid, unsigned long long int csize);
                    std::vector<struct PeerListElement>& get_elements();
                    unsigned short get_randezvous_port();
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
                    void receive_message(std::string push_resp_recv);
                private:
                    unsigned short _randezvous_port;
                    std::vector<struct PeerListElement> _elements;

            };

            class Request : public RequestHeader {
                private:
                    unsigned long long int _file_size;
                    std::string _dir_name;
                    std::string _file_name;
                public:
                    Request() : RequestHeader(std::string("file_push_request")) {}
                    Request(std::string fn , std::string d , unsigned long long int fs ): RequestHeader(std::string("file_push_request")), _file_size(fs), _dir_name(d), _file_name(fn) {}
                    std::string& get_file_name();
                    std::string& get_dir_name();
                    unsigned long long int get_file_size();
                    void send_mess(nar::Socket* skt,  nar::MessageTypes::FilePush::Response & resp);
                    void receive_message(std::string push_req_recv);
                    nlohmann::json test_json();
            };


        }
    }
}

#endif
