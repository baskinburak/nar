#include <nar/narnode/IPC/ipcclient.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <nar/narnode/Exception/exception.h>
#include <algorithm>
#include <nar/narnode/utility.h>


nar::IPCClient::IPCClient(std::string fp) {
    struct sockaddr_un temp;
    if(fp.size() >= sizeof(temp.sun_path)) {
        throw nar::Exception("narIPCClient::narIPCClient - File path is too long.");
    }
    file_path = fp;
}

void nar::IPCClient::connectServer() {
    struct sockaddr_un remote;
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        throw nar::Exception("narIPCClient::connect - Socket creation error.");
    }

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, file_path.c_str());

    int len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if(connect(sockfd, (struct sockaddr*) &remote, len) == -1) {
        close(sockfd);
        throw nar::Exception("narIPCClient::connect - Socket connect error.");
    }
}

void nar::IPCClient::sendRequest(std::string str) {
    int len = str.size();
    nar::send_int_sckt(sockfd, len);
    std::string ok = nar::get_string_sckt(sockfd, 2);

    if(ok != std::string("OK"))
        throw nar::Exception("narIPCClient::sendRequest - Receive OK error.");


    nar::send_string_sckt(sockfd, str, len);

    ok = nar::get_string_sckt(sockfd, 2);

    if(ok != std::string("OK"))
        throw nar::Exception("narIPCClient::sendRequest - Receive 2 OK error.");

}

std::string nar::IPCClient::getResponse() {
    char buf[129];
    int l;
    int len = nar::get_int_sckt(sockfd);
//    std::cout <<"len: " << len << std::endl;


    nar::send_string_sckt(sockfd, std::string("OK"), 2);

    std::string result = nar::get_string_sckt(sockfd, len);

    nar::send_string_sckt(sockfd, std::string("OK"), 2);
    return result;
}

void nar::IPCClient::closeConnection() {
    close(sockfd);
}

void nar::IPCClient::printLoop() {
    std::string response;
    while((response = getResponse()) != std::string("END")) {
        std::cout << response << std::endl;
    }
    closeConnection();
}
