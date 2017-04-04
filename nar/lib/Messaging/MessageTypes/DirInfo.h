#ifndef NAR_MESSDIRINFO_H
#define NAR_MESSDIRINFO_H

#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <string>
#include <vector>
namespace nar {
    namespace MessageTypes {
        namespace DirInfo {

            /*
             * Response the directory information
             *
             *
             * @author: Fatih Can Kurnaz
             * @privar: elements, std::vector<struct DirListElement>, holds information for each directory element
             * @tested: Yes
             * @todo: add exception for error status_codes
            */
            class Response : public ResponseHeader {
                public:
                    struct DirListElement {         // each directory item consists of those informations
                        std::string change_time;
                        long long int entity_id;
                        std::string entity_name;
                        unsigned long long int entity_size;
                        bool type; // true for DIR, false for FILE
                    };
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: e, std::vector<struct DirListElement>&, giving all the elements in one go
                     * @tested: Yes
                    */
                    Response(int statcode = -1, std::vector<struct DirListElement>& e) : ResponseHeader(statcode, std::string("get_dir_info")), elements(e) {}
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @tested: Yes
                    */
                    Response(int statcode = -1) : ResponseHeader(statcode, std::string("get_dir_info")) {}
                    /*
                     * gets the all directory elemets as vector
                     *
                     * @author: Fatih
                     * @return: std::vector<struct DirListElement>&, directory elements
                     * @tested: Yes
                    */
                    std::vector<struct DirListElement>& get_elements();
                    /*
                     * add new element to directory elements
                     *
                     * @author: Fatih
                     * @param: ele, struct DirListElement&, new element to add
                     * @tested: Yes
                    */
                    void add_element(struct DirListElement& ele);
                    /*
                     * add new element to directory elements
                     *
                     * @author: Fatih
                     * @param: ct, std::string, change time value
                     * @param: ei, long long int, entity id value
                     * @param: en, std::string, entity name value
                     * @param: s, unsigned long long int, entity size value
                     * @param: t, bool, entity type value
                     * @tested: Yes
                    */
                    void add_element(std::string ct, long long int ei, std::string en, unsigned long long int s, bool t);
                    /*
                     * send response message
                     *
                     * @author: Fatih
                     * @param: skt, nar::Socket *, socket pointer for sending data
                     * @tested: Yes
                    */
                    void send_mess(nar::Socket* skt);
                    /*
                     * receive response message
                     *
                     * @author: Fatih
                     * @param: dir_req_recv, nlohmann::json, that is the the json that contains the required data
                     * @tested: Yes
                    */
                    void receive_message(nlohmann::json dir_req_recv);
                    /*
                     * for testing and seeing what specific class
                     *
                     * @author: Fatih
                     * @return:  nlohmann::json, returns the all class data like how send message sends the json
                     * @tested: Yes
                    */
                    nlohmann::json test_json();
                private:
                    std::vector<struct DirListElement> elements;
            };


            /*
             * Requests specific directory information
             *
             * @author: Fatih
             * @privar: dir, std::string, holds the directory name for requesting that directories information
             * @tested: Yes
             * @todo: add exception for error status_codes
            */
            class Request : public RequestHeader {
                private:
                    std::string dir;

                public:


                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: d, std::string, holds the desired dir informaiton
                     * @tested: Yes
                    */
                    Request(std::string d  = std::string("")) : RequestHeader(std::string("get_dir_info")), dir(d) {}


                    /*
                     * Returns the directory name. User can edit this returned value
                     *
                     * @author: Fatih
                     * @param: d, std::string, holds the desired dir informaiton
                     * @tested: Yes
                    */
                    std::string& get_dir();


                    /*
                     * Creates the json and sends it
                     *
                     * @author: Fatih
                     * @param: skt, nar::Socket*, for sending data over the socket
                     * @param: resp, nar::MessageTypes::DirInfo::Response &, for receiving the respsonse data
                     * @todo: Will give socket as input to send directly
                     * @tested: Yes
                    */
                    void send_mess(nar::Socket* skt, nar::MessageTypes::DirInfo::Response & resp);


                    /*
                     * fills the class with the received information
                     *
                     * @author: Fatih
                     * @param: dir_req_recv, nlohmann::json, received json data
                     * @todo: maybe it can take socket directly
                     * @tested: Yes
                    */
                    void receive_message(nlohmann::json dir_req_recv);

                    /*
                     * for testing and seeing what specific class
                     *
                     * @author: Fatih
                     * @return:  nlohmann::json, returns the all class data like how send message sends the json
                     * @tested: Yes
                    */
                    nlohmann::json test_json();
            };

        }
    }
}

#endif
