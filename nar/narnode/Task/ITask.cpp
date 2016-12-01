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
    nar::send_message(skt, stringify);
    std::string resp = nar::get_message(skt);
    jsn = json::parse(resp);
    int stat = jsn["header"]["status-code"];
    std::string repl = jsn["header"]["reply-to"];
    return repl == "handshake" && stat == 200;
}
