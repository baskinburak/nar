#ifndef NAR_IPCCLIENT_H
#define NAR_IPCCLIENT_H

#include <string>

namespace nar {
    class IPCClient {
        private:
            std::string file_path;
            int sockfd;
        public:
            IPCClient(std::string fp);
            void connectServer();
            void sendRequest(std::string str);
            std::string getResponse();
            void closeConnection();
            void printLoop();
    };
}

#endif
