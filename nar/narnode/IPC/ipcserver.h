#ifndef NAR_IPCSERVER_H
#define NAR_IPCSERVER_H

#include <string>
namespace nar {
    class IPCServer {
        private:
            std::string file_path;
            int listen_socket;
        public:
            IPCServer(std::string fp);
            void initialize();
            int acceptConnection(); // returns a socket ident

    };
}

#endif
