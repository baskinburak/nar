#ifndef NAR_IPCRESPONSE_H
#define NAR_IPCRESPONSE_H

#include <string>

namespace nar {
    namespace Messagetypes {
        class IPCResponse {
            /*
             * IPC Response information
             *
             * @author: Dogu
             * @privar: long int progress, holds the information in integer like 40,65,
                        std::string process_name, states the information like pull,push,
                        long int status_code, states how the process is going,like error
                        std::string file_name, holds the file name
             * @tested: No
             * @todo: exceptions
            */
            private:
                long int progress;
                std::string process_name;
                long int status_code;
                std::string file_name;
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
                std::string get_process_name();
                /*
                 * Getter of status code
                 *
                 * @author: Dogu
                 * @tested: No
                */
                long int get_status_code();
                /*
                 * Getter of file name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                std::string get_file_name();

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
                void set_process_name(std::string pn);
                /*
                 * Setter of status code
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_status_code(long int sc);
                /*
                 * Setter of file name
                 *
                 * @author: Dogu
                 * @tested: No
                */
                void set_file_name(std::string fn);

                /*
                 * Constructor
                 *
                 * @author: Dogu
                 * @param:  long int prog, holds the information in integer like 40,65,
                            std::string pn, states the information like pull,push,
                            long int sc, states how the process is going,like error
                            std::string fn, holds the file name
                 * @tested: No
                */
                IPCResponse(long int prog, std::string pn, long int sc, std::string fn)
                            : progress(prog)
                            , process_name(pn)
                            , status_code(sc)
                            , file_name(fn)
                            { }
                /*
                 * sends the related info to other side
                 *
                 * @author: Dogu
                 * @param:  nar::Socket* skt
                 * @tested: No
                */
                nlohmann::json loop_send_message(nar::Socket* skt);
        };
    }
}

#endif
