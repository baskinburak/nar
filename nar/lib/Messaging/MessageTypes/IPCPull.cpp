#include "IPCPull.h"

std::string nar::MessageTypes::IPCPull::Request::get_file_name(){
    return _file_name;
}

std::string nar::MessageTypes::IPCPull::Request::get_cur_dir(){
    return _cur_dir;
}

void nar::MessageTypes::IPCPull::Request::set_file_name(std::string fn){
    this -> _file_name = fn;
    return;
}

void nar::MessageTypes::IPCPull::Request::set_cur_dir(std::string cd){
    this -> _cur_dir = cd;
    return;
}

nlohmann::json nar::MessageTypes::IPCPull::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "pull";
    json_to_sent["payload"]["file_name"] = _file_name;
    json_to_sent["payload"]["cur_dir"] = _cur_dir;
    return json_to_sent;
}

void nar::MessageTypes::IPCPull::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["_action"] = "pull";
    json_to_sent["payload"]["file_name"] = _file_name;
    json_to_sent["payload"]["cur_dir"] = _cur_dir;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCPull::Request::receive_message(nlohmann::json &js){
    this -> _action = js["header"]["action"];
    this -> _file_name = js["payload"]["file_name"];
    this -> _cur_dir = js["payload"]["cur_dir"];
    return;
}

nlohmann::json nar::MessageTypes::IPCPull::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
