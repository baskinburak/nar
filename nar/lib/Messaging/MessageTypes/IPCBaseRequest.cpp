#include "IPCBaseRequest.h"

std::string nar::Messagetypes::IPCBaseRequest::get_action_name() {
    return action_name;
}

void nar::Messagetypes::IPCBaseRequest::set_action_name(std::string an) {
    action_name = an;
    return;
}

nlohmann::json nar::Messagetypes::IPCBaseRequest::fillTheHead() {
    nlohmann::json header;
    header["action_name"] = action_name;
    return header;
}

void nar::Messagetypes::IPCBaseRequest::recvTheAction(nlohmann::json &recv){
    this -> action_name = recv["action_name"];
    return;
}

nlohmann::json nar::Messagetypes::IPCBaseRequest::get_myrequestjson() {
    nlohmann::json my_request_json;
    my_request_json["header"]["process_name"] = action_name;
    return my_request_json;
}

void nar::Messagetypes::IPCBaseRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = action_name;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::Messagetypes::IPCBaseRequest::print_loop(nar::Socket* skt) {
    while(true){
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(temp);
        if(received["header"]["process_name"] == std::string("END")){
            break;
        }
    }
    return;
}
