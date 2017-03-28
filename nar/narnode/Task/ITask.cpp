#include <nar/narnode/Task/ITask.h>
#include <nar/lib/json.hpp>
#include <nar/narnode/utility.h>
using namespace nlohmann;

bool nar::task::ITask::handshake(nar::Socket& skt, nar::Global* globals) {
    std::string username = globals->get_username();
    nar::MessageTypes::Handshake::Request hand_req(username,globals->get_macId())
    nar::MessageTypes::Handshake::Response hand_resp(999);
    hand_req.send_mess(skt,hand_resp);
    std::string action = hand_resp.get_action();
    int stat = hand_resp.get_status_code();
    return action == "handshake" && stat == 200;
}
