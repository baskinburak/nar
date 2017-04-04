#include "IPCLs.h"

std::string nar::MessageTypes::IPCLs::Request::get_dir_name(){
    return _dir_name;
}

void nar::MessageTypes::IPCLs::Request::set_dir_name(std::string dn){
    this->_dir_name = dn;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "ls";
    json_to_sent["payload"]["dir_name"] = _dir_name;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["dir_name"] = this->_dir_name;
    return jsn;
}

void nar::MessageTypes::IPCLs::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_dir_name = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCLs::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}





nlohmann::json nar::MessageTypes::IPCLs::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
