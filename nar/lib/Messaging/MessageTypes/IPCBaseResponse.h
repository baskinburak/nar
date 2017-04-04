#ifndef NAR_IPCBASERESPONSE_H
#define NAR_IPCBASERESPONSE_H

#include <string>
#include <nar/lib/nlohJson/json.hpp>

#include <nar/lib/Socket/Socket.h>
#include <nar/narnode/utility.h>

namespace nar {
    namespace MessageTypes {
        class IPCBaseResponse {
            /*
             * IPCBaseResponse information
             *
             * @author: Dogu
             * @privar: long int progress, holds the information in integer like 40,65,
                        std::string reply_to, states the information like pull,push,
                        long int status_code, states how the process is going,like error
             * @tested: No
             * @todo: exceptions
            */
            private:
                long int _progress;
                std::string _reply_to;
                long int _status_code;
            public:
                /*
                 * Getter of progress
                 *
                 * @author: Dogu
                 * @tested: No
                */
                long int get_progress();
                /*
                 * Getter of process name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_reply_to();
                /*
                 * Getter of status code
                 *
                 * @author: Dogu
                 * @tested: No
                */
                long int get_status_code();
                /*
                 * Setter of progress
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_progress(long int p);
                /*
                 * Setter of process name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_reply_to(std::string pn);
                /*
                 * Setter of status code
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_status_code(long int sc);
                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  long int prog, holds the information in integer like 40,65,
                            std::string pn, states the information like pull,push,
                            long int sc, states how the process is going,like error
                                    200->success
                                    300->fail
                                    400->error
                                    these can be changed
                 * @tested: No
                */
                IPCBaseResponse(long int prog, std::string pn, long int sc)
                            : _progress(prog)
                            , _reply_to(pn)
                            , _status_code(sc)
                            { }
                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  long int prog, holds the information in integer like 40,65,
                            long int sc, states how the process is going,like error
                                    200->success
                                    300->fail
                                    400->error
                                    these can be changed
                 * @tested: No
                */
                IPCBaseResponse(long int prog,long int sc)
                            : _progress(prog)
                            , _status_code(sc)
                            { }
                /*
                * fill the head with _action name
                *
                * @author: Dogu
                * @param: None
                * @tested: No
                *
                */
                nlohmann::json fillTheHead();
                /*
                * returns the half-filled
                *
                * @author: Dogu
                * @param: nlohmann::json
                * @tested: No
                *
                */
                void recvThe_action(nlohmann::json &recv);

                /*
                 * return json,it is actually for testing purpose.
                 *
                 * @author: Dogu
                 * @param: None
                 * @tested: No
                 *
                */
                nlohmann::json give_myresponsejson();
                /*
                 * sends the related info to other side
                 *
                 * @author: Dogu
                 * @param:  nar::Socket* skt, it is socket pointer basically
                            progress, update the progress by giving integer value
                 * @tested: No
                */
                void send_message_progress(nar::Socket* skt, int progress);
                /*
                 * sends "END" message
                 *
                 * @author: Dogu
                 * @param:  nar::Socket* skt, it is socket pointer basically
                 * @tested: No
                */
                void send_message_end(nar::Socket* skt);
        };
    }
}

#endif
