#ifndef NAR_MESSDIRINFO_H
#define NAR_MESSDIRINFO_H

#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"
#include <string>
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace DirInfo {
            /*
             * Requests specific directory information
             *
             *
             *
             * @author: Fatih Can Kurnaz
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
                    Request(std::string d) : RequestHeader(std::string("get_dir_info")), dir(d) {}


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
                     * @todo: Will give socket as input to send directly
                     * @tested: Yes
                    */
                    void sendMessage();


                    /*
                     * fills the class with the received information
                     *
                     * @author: Fatih
                     * @param: dir_req_recv, nlohmann::json, received json data
                     * @todo: maybe it can take socket directly
                     * @tested: Yes
                    */
                    void receiveMessage(nlohmann::json dir_req_recv);

                    /*
                     * Used for testing purposes
                     *
                     * @author: Fatih
                     * @tested: Yes
                    */
                    nlohmann::json test_json();
            };
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
                        std::string entity_id;
                        std::string entity_name;
                        unsigned long long int entity_size;
                        bool type; // true for DIR, false for FILE
                    };
                    Response(int statcode, std::vector<struct DirListElement>& e) : ResponseHeader(statcode, std::string("get_dir_info")), elements(e) {}
                    Response(int statcode) : ResponseHeader(statcode, std::string("get_dir_info")) {}
                    std::vector<struct DirListElement>& get_elements();
                    void add_element(struct DirListElement& ele);
                    void add_element(std::string ct, std::string ei, std::string en, unsigned long long int s, bool t);
                    void sendMessage();
                    void receiveMessage(nlohmann::json dir_resp_recv);
                    nlohmann::json test_json();
                private:
                    std::vector<struct DirListElement> elements;
            };
        }
    }
}

#endif
