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
#include <nar/narnode/Socket/ClientSocket.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <cstdlib>

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
        nar::ClientSocket skt("127.0.0.1", 12345);
        nar::FileKeeper file_keeper(push_file);

        char* buf = (char*) malloc(sizeof(char)*512);
        int step = 512;
        int start = 0;
        int len;
        while((len = file_keeper.getBytes(start, step, buf)) > 0) {
            skt.send(buf, len);            
            start+=len;
        }
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
