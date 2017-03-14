#ifndef NAR_IPCREQUEST_H
#define NAR_IPCREQUEST_H

#include <string>

namespace nar {
    namespace Messagetypes {
        class IPCRequest {
            /*
             * IPC Request information
             *
             * @author: Dogu
             * @privar: std::string action_name, holds the action name,
             *          std::file_name, holds the action name,
             *          std::user_name, holds the user name
             * @tested: No
             * @todo: exceptions
            */
            private:
                std::string action_name;
                std::string file_name;
                std::string user_name;
                bool isEnd;
            public:
                /*
                 * Getter of action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_action_name();
                /*
                 * Getter of file name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_file_name();
                /*
                 * Getter of user name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_user_name();
                /*
                 * Setter of action name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_action_name(std::string an);
                /*
                 * Setter of file name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_file_name(std::string fn);
                /*
                 * Setter of user name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_user_name(std::string un);

                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  std::string an, holds the name of the action,
                            std::string fn, holds the name of the file
                 * @tested: No
                */
                IPCRequest(std::string an, std::string fn, std::string un) {
                    action_name = an;
                    file_name = fn;
                    user_name = un;
                    isEnd = false;
                }

                /*
                 * sends the related info to other side
                 *
                 * @author: Dogu
                 * @param:  nar::Socket* skt
                 * @tested: No
                */
                void send_action(nar::Socket* skt);
        };
    }
}

#endif
