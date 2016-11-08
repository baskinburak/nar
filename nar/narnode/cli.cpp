#include <iostream>
#include <nar/narnode/exception.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <cstdlib>
#include <nar/narnode/ipcclient.h>
#include <fstream>
#define NAR_IPC_FILE "/tmp/nar_ipc"


void nar_push_file(std::string file_name) {
    char* real_path = realpath(file_name.c_str(), NULL);
    file_name = std::string(real_path);
    std::cout << file_name << std::endl;

    std::ifstream f(file_name.c_str());
    if(!f.good()) {
        std::cout << file_name  << " could not be opened." << std::endl;
        return;
    }

    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value action;
    action.SetString("push");

    rapidjson::Value filename;
    filename.SetString(file_name.c_str(), file_name.size(), allocator);

    doc.AddMember("action", action, allocator);
    doc.AddMember("file", filename, allocator);

    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string stringify(buffer.GetString());

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(stringify);
    std::string response = ipc_client.getResponse();


    std::cout << response << std::endl;
}

int main(int argc, char* argv[]){
    if(argc < 2) {
        return 0;
    }

    std::string first_arg(argv[1]);

    if(first_arg == std::string("push")) {
        if(argc < 3) {
            return 0;
        }

        std::string file_name(argv[2]);
        nar_push_file(file_name);
    }

    return 0;
}
