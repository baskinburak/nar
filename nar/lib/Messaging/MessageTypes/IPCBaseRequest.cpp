#include "IPCBaseRequest.h"

std::string nar::MessageTypes::IPCBaseRequest::get_action_name() {
    return action_name;
}

void nar::MessageTypes::IPCBaseRequest::set_action_name(std::string an) {
    action_name = an;
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseRequest::fillTheHead() {
    nlohmann::json header;
    header["action_name"] = action_name;
    return header;
}

void nar::MessageTypes::IPCBaseRequest::recvTheAction(nlohmann::json &recv){
    this -> action_name = recv["action_name"];
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseRequest::get_myrequestjson() {
    nlohmann::json my_request_json;
    my_request_json["header"]["process_name"] = action_name;
    return my_request_json;
}
/*
void nar::MessageTypes::IPCBaseRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = action_name;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCBaseRequest::print_loop(nar::Socket* skt) {
    while(true){
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(temp);
        if(received["header"]["process_name"] == std::string("END")){
            break;
        }
    }
    return;
}
*/
