#include "IPCRegister.h"

std::string nar::MessageTypes::IPCRegister::Request::get_user_name(){
    return user_name;
}

void nar::MessageTypes::IPCRegister::Request::set_user_name(std::string un){
    this -> user_name = un;
    return;
}

nlohmann::json nar::MessageTypes::IPCRegister::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "register";
    json_to_sent["payload"]["user_name"] = user_name;
    return json_to_sent;
}
/*
void nar::MessageTypes::IPCRegister::Request::send__action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["_action"] = "register";
    json_to_sent["payload"]["user_name"] = user_name;
    send_message(skt, json_to_sent.dump());
    return;
}
*/
nlohmann::json nar::MessageTypes::IPCRegister::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
