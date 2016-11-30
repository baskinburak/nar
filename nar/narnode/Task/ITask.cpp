#include <nar/narnode/Task/ITask.h>
#include <nar/lib/json.hpp>
#include <nar/narnode/utility.h>
using namespace nlohmann;

bool nar::task::ITask::handshake(nar::Socket& skt, nar::Global* globals) {
    std::string username = globals->get_username();
    json jsn;

    jsn["header"]["action"] = "handshake";
    jsn["header"]["channel"] = "ps";
    jsn["payload"]["username"] = username;

    std::string stringify = jsn.dump();
    std::string len = std::to_string((int)stringify.size());

    skt.send((char*)len.c_str(), len.size());
    skt.send((char*)" ", 1);
    skt.send((char*)stringify.c_str(), stringify.size());



    std::string resp = nar::get_message(skt);
    std::cout<<"1"<<std::endl;
    jsn = json::parse(resp);
    std::cout<<"2"<<std::endl;
    int stat = jsn["header"]["status-code"];
    std::string repl = jsn["header"]["reply-to"];
    std::cout<<"3"<<std::endl;


    std::cout << stat << std::endl;
    std::cout << repl << std::endl;
    std::cout<<"4"<<std::endl;
    return repl == "handshake" && stat == 200;
}
