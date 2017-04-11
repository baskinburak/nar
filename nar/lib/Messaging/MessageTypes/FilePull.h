#ifndef NAR_MESSFILEPULL_H
#define NAR_MESSFILEPULL_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <vector>
namespace nar {
    namespace MessageTypes {
        namespace FilePull {
            /*
             * Response of the file pull key messages
             *
             *
             * @author: Fatih
             * @privar: _randevous_port, unsigned short, for keeping randevous information
             * @privar: elements, std::vector<struct PeerListElement>, for keeping file pull element informations
             * @tested: Yes
             * @todo: add exception for error status_codes
            */
            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string machine_id;
                        unsigned long long int chunk_id;
                        long long int stream_id;
                        unsigned long long int chunk_size;

                        bool operator < (const PeerListElement& str) const
                        {
                           return (chunk_id < str.chunk_id);
                        }
                    };
                    Response(): ResponseHeader(-1, std::string("file_pull_request")){}
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: rport, unsigned short, aes key information
                     * @tested: Yes
                    */
                    Response(int statcode , unsigned short  rport): ResponseHeader(statcode, std::string("file_pull_request")), _rendezvous_port(rport) {}
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: rport, unsigned short, aes key information
                     * @tested: Yes
                    */
                    Response(int statcode  , unsigned short  rport,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_pull_request")), _rendezvous_port(rport),  _elements(eles) {}
                    /*
                     * adds new elements to pull file message
                     *
                     * @author: Fatih
                     * @param: ele, struct PeerListElement&, adds new elements
                     * @tested: Yes
                    */
                    void add_element(struct PeerListElement& ele);
                    /*
                     * adds new elements to pull file message
                     *
                     * @author: Fatih
                     * @param: mid, std::string, machine id value
                     * @param: cid, unsigned long long int, chunk id value
                     * @param: sid, long long int, stream id value
                     * @param: csize, unsigned long long int, chunk size value
                     * @tested: Yes
                    */
                    void add_element(std::string mid, unsigned long long int cid, long long int sid, unsigned long long int csize);
                    /*
                     * gives the elements of the peerlist
                     *
                     * @author: Fatih
                     * @return: std::vector<struct PeerListElement> &, all the elements
                     * @tested: Yes
                    */
                    std::vector<struct PeerListElement>& get_elements();
                    /*
                     * gives the private randevous_port
                     *
                     * @author: Fatih
                     * @return: unsigned short, value of the randevous_port
                     * @tested: Yes
                    */
                    unsigned short get_rendezvous_port();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
                private:
                    unsigned short _rendezvous_port;
                    std::vector<struct PeerListElement> _elements;



            };


            class Request : public RequestHeader {
                private:
                    std::string _dir_name;
                    std::string _file_name;

                public:
                    Request() :RequestHeader(std::string("file_pull_request")) {}
                    Request(std::string fn , std::string d ): RequestHeader(std::string("file_pull_request")),  _dir_name(d), _file_name(fn) {}
                    std::string& get_file_name();
                    std::string& get_dir_name();
                    void send_mess(nar::Socket* skt, nar::MessageTypes::FilePull::Response & resp);
                    void receive_message(std::string& pull_req_recv);
                    nlohmann::json test_json();
            };


        }
    }
}

#endif
