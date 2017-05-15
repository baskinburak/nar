//
// Created by fatih on 09.05.2017.
//

#ifndef NARMESS_DAEMONSHUTDOWN_H
#define NARMESS_DAEMONSHUTDOWN_H

#include <string>
#include <vector>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <nar/lib/Exception/Exception.h>
namespace nar {
    namespace MessageTypes {
        namespace DaemonShutdown {


            /*
             * Requests specific directory information
             *
             * @author: Fatih
            */
            class Request : public RequestHeader {
            private:
                std::string _mac_id;
            public:
                /*
                 * Constructor
                 *
                 * @author: Fatih
                 * @tested: Yes
                */
                Request(): RequestHeader(std::string("daemon_shutdown")) {}
                /*
                 * Constructor
                 *
                 * @author: Fatih
                 * @tested: Yes
                */
                Request(std::string mac_id): RequestHeader(std::string("daemon_shutdown")), _mac_id(mac_id) {}
                /*
                 * Creates the json and sends it
                 *
                 * @author: Fatih
                 * @param: skt, nar::Socket*, for sending data over the socket
                */
                void send_mess(nar::Socket* skt);
                /*
                 * Returns machine id
                 *
                 * @author: Fatih
                 *
                */
                std::string & get_machine_id();
                /*
                 * fills the class with the received information
                 *
                 * @author: Fatih
                 * @param: message, std::string, received json data
                */
                void receive_message(std::string& message);
                /*
                 * for testing and seeing what specific class
                 *
                 * @author: Fatih
                 * @return:  nlohmann::json, returns the all class data like how send message sends the json
                */
                nlohmann::json test_json();
            };


        }
    }
}


#endif //NARMESS_DAEMONSHUTDOWN_H
