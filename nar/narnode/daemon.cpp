#include <nar/narnode/ipcserver.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main() {
    nar::IPCServer server(std::string("nar_rpc"));
    server.initialize();

    while(1) {
        int sockfd = server.acceptConnection();

        char buf[128];

        int n = recv(sockfd, buf, 128, 0);
        std::cout << n << " " << buf << std::endl;
        close(sockfd);
    }
}
