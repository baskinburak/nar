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
                    };
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: rport, unsigned short, aes key information
                     * @tested: Yes
                    */
                    Response(int statcode = -1, unsigned short  rport = 0): ResponseHeader(statcode, std::string("file_pull_request")), _randevous_port(rport) {}
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: rport, unsigned short, aes key information
                     * @tested: Yes
                    */
                    Response(int statcode = -1, unsigned short  rport = 0,  std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_pull_request")), _randevous_port(rport),  elements(eles) {}
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
                    unsigned short get_randevous_port();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
                private:
                    unsigned short _randevous_port;
                    std::vector<struct PeerListElement> elements;



            };


            class Request : public RequestHeader {
                private:
                    std::string dir;
                    std::string file_name;

                public:
                    Request(std::string fn = std::string(""), std::string d = std::string("")): RequestHeader(std::string("file_pull_request")),  dir(d), file_name(fn) {}
                    std::string& get_filename();
                    std::string& get_dir();
                    void send_mess(nar::Socket* skt, nar::MessageTypes::FilePull::Response & resp);
                    void receive_message(nlohmann::json pull_req_recv);
                    nlohmann::json test_json();
            };


        }
    }
}

#endif
