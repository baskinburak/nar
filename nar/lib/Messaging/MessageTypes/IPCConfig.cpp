#include "IPCConfig.h"

std::string nar::MessageTypes::IPCConfig::Request::get_var(){
    return _var;
}

std::string nar::MessageTypes::IPCConfig::Request::get_value(){
    return _value;
}

void nar::MessageTypes::IPCConfig::Request::set_var(std::string var){
    this -> _var = var;
    return;
}

void nar::MessageTypes::IPCConfig::Request::set_value(std::string value){
    this -> _value = value;
    return;
}

nlohmann::json nar::MessageTypes::IPCConfig::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "config";
    json_to_sent["payload"]["var"] = _var;
    json_to_sent["payload"]["value"] = _value;
    return json_to_sent;
}
/*
void nar::MessageTypes::IPCConfig::Request::send__action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["_action"] = "config";
    json_to_sent["payload"]["var"] = _var;
    json_to_sent["payload"]["value"] = _value;
    send_message(skt, json_to_sent.dump());
    return;
}
*/
nlohmann::json nar::MessageTypes::IPCConfig::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
