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
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <cstdlib>
#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/Task/LS.h>
#include <nar/narnode/Task/PullFile.h>
#include <nar/narnode/global.h>
#include <nar/lib/json.hpp>

void keepalive(nar::Global* globals) {
    while(true) {
        std::cout << "asdf" << std::endl;
    }
}

void handle_cli_ipc(int sockfd, nar::Global* globals) {
    char buf[129];
    int len = nar::get_int_sckt(sockfd);
    nar::send_string_sckt(sockfd, std::string("OK"), 2);
    std::string json = nar::get_string_sckt(sockfd, len);

    std::cout << json << std::endl;

    rapidjson::Document doc;
    doc.Parse(json.c_str());
    std::string action(doc["action"].GetString());


    if(action == std::string("push")) {
        nar::task::PushFile task(doc["file"].GetString());
        task.run(sockfd, globals);
    } else if(action == std::string("ls")) {
        nar::task::LS task;
        task.run(sockfd, globals);
    } else if(action == std::string("pull")) { //todo
        nar::task::PullFile task(doc["file"].GetString());
        task.run(sockfd, globals);
    }

    close(sockfd);
}

int main() {
    nar::IPCServer cli_server(std::string("/tmp/nar_ipc"));
    cli_server.initialize();
    nar::Global* globals = new nar::Global();
    globals->set_username(std::string("nar_admin"));
    

    std::thread keepalv(keepalive, globals);
    keepalv.detach();

    while(true) {
        int sockfd = cli_server.acceptConnection();
        std::thread ipc_thread(handle_cli_ipc, sockfd, globals);
        ipc_thread.detach();
    }
}
