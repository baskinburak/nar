#ifndef NAR_IPCBASEREQUEST_H
#define NAR_IPCBASEREQUEST_H

#include <string>

namespace nar {
    namespace Messagetypes {
        class IPCBaseRequest {
            /*
             * IPCBaseRequest information
             *
             * @author: Dogu
             * @privar: std::string action_name, holds the action name
             * @tested: No
             * @todo: exceptions
            */
            private:
                std::string action_name;
            public:
                /*
                 * Getter of action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_action_name();
                /*
                 * Setter of action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_action_name(std::string an);
                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  std::string an, holds the name of the action,
                 * @tested: No
                */
                IPCBaseRequest(std::string an) : action_name(an) {}
                /*
                 * base class fills the header and returns the half-filled json
                 *
                 * @author: Dogu
                 * @param: None
                 * @tested: No
                */
                nlohmann::json fillTheHead();
                /*
                 * gets the action
                 *
                 * @author: Dogu
                 * @param: nlohmann::json
                 * @tested: No
                */
                void recvTheAction(nlohmann::json &recv);
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
                void send_action(nar::Socket* skt);
                /*
                 * get a message
                 *
                 * @author: Dogu
                 * @param:  None
                 * @tested: No
                */
                void print_loop(nar::Socket* skt);
        };
    }
}

#endif
