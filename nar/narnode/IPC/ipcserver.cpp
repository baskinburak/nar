#include <nar/narnode/IPC/ipcserver.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>
#include <unistd.h>
#include <nar/narnode/Exception/exception.h>

nar::IPCServer::IPCServer(std::string fp) {
    struct sockaddr_un temp;
    if(fp.size() >= sizeof(temp.sun_path)) {
        throw nar::Exception("narIPCServer::narIPCServer - File path is too long.");
    }
    file_path = fp;
}

void nar::IPCServer::initialize() {
    if((listen_socket = listen_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        throw nar::Exception("narIPCServer::initializeSocket - Socket creation error.");
    }

    struct sockaddr_un local;
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, file_path.c_str());
    unlink(local.sun_path);

    int len = strlen(local.sun_path) + sizeof(local.sun_family);

    if(bind(listen_socket, (struct sockaddr*)&local, len) == -1) {
        close(listen_socket);
        throw nar::Exception("narIPCServer::initializeSocket - Socket bind error.");
    }

    if(listen(listen_socket, 5) == -1) {
        close(listen_socket);
        throw nar::Exception("narIPCServer::initializeSocket - Socket listen error.");
    }
}

int nar::IPCServer::acceptConnection() {
    struct sockaddr_un remote;
    int len = sizeof(remote);
    int acc_socket;
    if((acc_socket = accept(listen_socket, (struct sockaddr*) &remote, (socklen_t *)&len)) == -1) {
        throw nar::Exception("narIPCServer::accept - Accept error.");
    }

    return acc_socket;
}
