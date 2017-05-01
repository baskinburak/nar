#ifndef NAR_MESSAESKEY_H
#define NAR_MESSAESKEY_H
#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <nar/lib/Exception/Exception.h>
namespace nar {
    namespace MessageTypes {
        namespace AesKey {
            /*
             * Response of the aes key messages
             *
             *
             * @author: Fatih
             * @privar: _aes, std::string, for holding aes key information
             * @tested: Yes
             * @todo: add exception for error status_codes
            */

            class Response : public ResponseHeader {
                private:
                    std::string _aes;
                public:
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @param: statcode, int, status code of the response message
                     * @param: aes, std::string, aes key information
                     * @tested: Yes
                    */
                    Response() {
                        ResponseHeader(-1,std::string("get_aes_key"));
                        _aes = "";
                    }
                    Response(int statcode, std::string aes ) : ResponseHeader(statcode, std::string("get_aes_key")), _aes(aes) {}
                    /*
                     * for getting aes key
                     *
                     * @author: Fatih
                     * @return: std::string , returns the private aes key
                     * @tested: Yes
                    */
                    std::string & get_aes_key();
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
                     * @param: keep_resp_recv, nlohmann::json, that is the the json that contains the required data
                     * @tested: Yes
                    */
                    void receive_message(nlohmann::json keep_resp_recv);
                    /*
                     * for testing and seeing what specific class
                     *
                     * @author: Fatih
                     * @return:  nlohmann::json, returns the all class data like how send message sends the json
                     * @tested: Yes
                    */
                    nlohmann::json test_json();
            };


            /*
             * Requests specific directory information
             *
             * @author: Fatih
             * @tested: Yes
             * @todo: add exception for error status_codes
            */
            class Request : public RequestHeader {
                public:
                    /*
                     * Constructor
                     *
                     * @author: Fatih
                     * @tested: Yes
                    */
                    Request(): RequestHeader(std::string("get_aes_key")) {}
                    /*
                     * Creates the json and sends it
                     *
                     * @author: Fatih
                     * @param: skt, nar::Socket*, for sending data over the socket
                     * @param: resp, nar::MessageTypes::AesKey::Response &, for receiving the respsonse data
                     * @todo: Will give socket as input to send directly
                     * @tested: Yes
                    */
                    void send_mess(nar::Socket* skt, nar::MessageTypes::AesKey::Response & resp);
                    /*
                     * fills the class with the received information
                     *
                     * @author: Fatih
                     * @param: keep_req_recv, nlohmann::json, received json data
                     * @todo: maybe it can take socket directly
                     * @tested: Yes
                    */
                    void receive_message(nlohmann::json keep_req_recv);
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
