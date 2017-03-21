#include "IPCRequest.h"

std::string nar::Messagetypes::IPCRequest::get_action_name() {
    return action_name;
}

std::string nar::Messagetypes::IPCRequest::get_file_name() {
    return file_name;
}

std::string nar::Messagetypes::IPCRequest::get_user_name() {
    return user_name;
}

bool nar::Messagetypes::IPCRequest::get_isEnd() {
    return isEnd;
}

void nar::Messagetypes::IPCRequest::set_action_name(std::string an) {
    action_name = an;
}

void nar::Messagetypes::IPCRequest::set_file_name(std::string fn) {
    file_name = fn;
}

void nar::Messagetypes::IPCRequest::set_user_name(std::string un) {
    user_name = un;
}

void nar::Messagetypes::IPCRequest::set_isEnd(bool ie) {
    isEnd = ie;
}

nlohmann::json nar::Messagetypes::IPCRequest::get_myrequestjson() {
    nlohmann::json my_request_json;
    my_request_json["header"]["process_name"] = action_name;
    my_request_json["payload"]["file_name"] = file_name;
    my_request_json["payload"]["user_name"] = user_name;
    return my_request_json;
}

void nar::Messagetypes::IPCRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = action_name;
    json_to_sent["payload"]["file_name"] = file_name;
    json_to_sent["payload"]["user_name"] = user_name;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::Messagetypes::IPCRequest::print_loop(nar::Socket* skt) {
    while(true){
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(temp);
        if(received["header"]["process_name"] == std::string("END")){
            isEnd = true;
            break;
        }
    }
    return;
}
