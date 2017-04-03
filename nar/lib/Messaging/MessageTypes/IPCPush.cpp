#include "IPCPush.h"

std::string nar::MessageTypes::IPCPush::Request::get_file_name(){
    return _file_name;
}

void nar::MessageTypes::IPCPush::Request::set_file_name(std::string fn){
    this -> _file_name = fn;
    return;
}

nlohmann::json nar::MessageTypes::IPCPush::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "push";
    json_to_sent["payload"]["file_name"] = _file_name;
    return json_to_sent;
}

void nar::MessageTypes::IPCPush::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["_action"] = "push";
    json_to_sent["payload"]["file_name"] = _file_name;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCPush::Request::receive_message(nlohmann::json &js){
    this -> _action = js["header"]["action"];
    this -> _file_name = js["payload"]["file_name"];
    return;
}

nlohmann::json nar::MessageTypes::IPCPush::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
