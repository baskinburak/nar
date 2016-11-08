#include <nar/narnode/IPC/ipcserver.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <nar/narnode/Exception/exception.h>
#include <nar/narnode/utility.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>

void handle_cli_ipc(int sockfd) {
    char buf[129];
    int len = nar::get_int_sckt(sockfd);
    nar::send_string_sckt(sockfd, std::string("OK"), 2);
    std::string json = nar::get_string_sckt(sockfd, len);

    std::cout << json << std::endl;

    rapidjson::Document doc;
    doc.Parse(json.c_str());
    std::string action(doc["action"].GetString());

    if(action == std::string("push")) {
        std::string push_file(doc["file"].GetString());
    }


    close(sockfd);
}

int main() {
    nar::IPCServer cli_server(std::string("/tmp/nar_ipc"));
    cli_server.initialize();

    while(true) {
        int sockfd = cli_server.acceptConnection();
        std::thread ipc_thread(handle_cli_ipc, sockfd);
        ipc_thread.detach();
    }
}
