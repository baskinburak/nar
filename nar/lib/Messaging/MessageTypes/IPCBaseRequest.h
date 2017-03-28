#ifndef NAR_IPCBASEREQUEST_H
#define NAR_IPCBASEREQUEST_H

#include <string>
#include <nar/lib/nlohJson/json.hpp>

namespace nar {
    namespace MessageTypes {
        class IPCBaseRequest {
            /*
             * IPCBaseRequest information
             *
             * @author: Dogu
             * @privar: std::string _action, holds the _action name
             * @tested: No
             * @todo: exceptions
            */
            private:
                std::string _action;
            public:
                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  std::string an, holds the name of the _action,
                 * @tested: No
                */
                IPCBaseRequest(std::string an) : _action(an) {}
                /*
                 * Getter of _action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_action();
                /*
                 * Setter of _action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_action(std::string an);

                /*
                 * base class fills the header and returns the half-filled json
                 *
                 * @author: Dogu
                 * @param: None
                 * @tested: No
                */
                nlohmann::json fillTheHead();
                /*
                 * gets the _action
                 *
                 * @author: Dogu
                 * @param: nlohmann::json
                 * @tested: No
                */
                void recvThe_action(nlohmann::json &recv);
                /*
                 * return all things in a json
                 *
                 * @author: Dogu
                 * @param: None
                 * @tested: No
                */
                nlohmann::json get_myrequestjson();
                /*
                 * sends the related info to other side
                 *
                 * @author: Dogu
                 * @param:  nar::Socket* skt
                 * @tested: No
                */
                //simdilik void send__action(nar::Socket* skt);
                /*
                 * get a message
                 *
                 * @author: Dogu
                 * @param:  None
                 * @tested: No
                */
                //simdilik void print_loop(nar::Socket* skt);
        };
    }
}

#endif
