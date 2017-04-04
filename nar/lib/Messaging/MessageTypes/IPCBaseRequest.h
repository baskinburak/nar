#ifndef NAR_IPCBASEREQUEST_H
#define NAR_IPCBASEREQUEST_H

#include <string>
#include <nar/lib/nlohJson/json.hpp>
#include <nar/lib/Socket/Socket.h>
#include <nar/narnode/utility.h>

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
                std::string _username;
                std::string _password;
                std::string _current_directory;
            public:
                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  std::string an, holds the name of the _action,
                 * @tested: No
                */
                IPCBaseRequest(std::string an, std::string un, std::string pw, std::string cd) : _action(an), _username(un), _password(pw), _current_directory(cd) { }
                IPCBaseRequest(std::string an) : _action(an) { }
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
                std::string& get_username();
                std::string& get_current_directory();
                std::string& get_password();
                void set_username(std::string& un);
                void set_current_directory(std::string& cd);
                void set_password(std::string& pw);
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
                 * returns the json form of base request
                */
                nlohmann::json generate_json();

                /*
                 * populates object from given json
                */
                void populate_object(nlohmann::json& jsn);

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
