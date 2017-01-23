#include <iostream>
#include <nar/lib/Exception/exception.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <cstdlib>
#include <nar/narnode/IPC/ipcclient.h>
#include <fstream>
#include <unistd.h>
#include <nar/lib/json.hpp>
#define NAR_IPC_FILE "/tmp/nar_ipc"

using namespace nlohmann;

void nar_push_file(std::string file_name) {
/*
{
    "action": "push",
    "file": file_path
}
*/

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

    nar::IPCClient ipc_client(NAR_IPC_FILE); //NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(stringify);

    ipc_client.printLoop();
}

void nar_ls(std::string dir_name) {
/*
{
    "action": "ls"
    "dir": dir
}
*/
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value action;
    action.SetString("ls");

    rapidjson::Value dir;
    dir.SetString(dir_name.c_str(), dir_name.size(), allocator);

    doc.AddMember("action", action, allocator);
    doc.AddMember("dir", dir, allocator);

    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string stringify(buffer.GetString());

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(stringify);

    ipc_client.printLoop();
}

void nar_pull_file(std::string file_name) {
/*
{
    "action": "pull",
    "file": file_name,
    "cur_id": cur_dir
}
*/


    std::cout << file_name << std::endl;


    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();


    char cwd[1024];
    char* dum = getcwd(cwd, sizeof(cwd));
    std::string cwdss(cwd);
    rapidjson::Value cwds;
    cwds.SetString(cwdss.c_str(), cwdss.size(), allocator);


    rapidjson::Value action;
    action.SetString("pull");

    rapidjson::Value filename;
    filename.SetString(file_name.c_str(), file_name.size(), allocator);

    doc.AddMember("action", action, allocator);
    doc.AddMember("file", filename, allocator);
    doc.AddMember("cur_dir", cwds, allocator);


    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string stringify(buffer.GetString());

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(stringify);

    ipc_client.printLoop();
}

void nar_register(std::string uname) {
/*
{
    "action": "register",
    "username": username
}
*/

    json jsn;
    jsn["action"] = "register";
    jsn["username"] = uname;

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(jsn.dump());


    ipc_client.printLoop();
}

void nar_config(std::string var, std::string value) {
/*
{
    "action": "config",
    "var": var,
    "value": value
}
*/

    json jsn;
    jsn["action"] = "config";
    jsn["var"] = var;
    jsn["value"] = value;

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(jsn.dump());

    ipc_client.printLoop();
}

void nar_status() {
/*
{
    "action": "status"
}
*/

    json jsn;
    jsn["action"] = "status";

    nar::IPCClient ipc_client(NAR_IPC_FILE);
    ipc_client.connectServer();
    ipc_client.sendRequest(jsn.dump());

    ipc_client.printLoop();
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
    } else if(first_arg == std::string("ls")) {
        if(argc < 3) {
            nar_ls(std::string(""));
        }
        else if(argc < 4){
            std::string dir_name(argv[2]);
            nar_ls(dir_name);
        }

    } else if(first_arg == std::string("pull")) {
        if(argc < 3) {
            return 0;
        }

        std::string file_name(argv[2]);
        nar_pull_file(file_name);
    } else if(first_arg == std::string("register")) {
        std::string uname;
        std::cout << "Enter a username: ";
        std::cin >> uname;
        nar_register(uname);
    } else if(first_arg == std::string("config")) {
        if(argc < 3) {
            return 0;
        }
        for(int i=2; i<argc; i++) {
            std::string conf(argv[i]);
            int eq = conf.find('=');
            std::string left = conf.substr(0, eq);
            std::string right = conf.substr(eq+1);
            nar_config(left, right);
        }
    } else if(first_arg == std::string("status")) {
        nar_status();
    }

    return 0;
}
