#include "IPCLs.h"

std::string nar::MessageTypes::IPCLs::Request::get_dir_name(){
    return dir_name;
}

void nar::MessageTypes::IPCLs::Request::set_dir_name(std::string dn){
    this -> dir_name = dn;
    return;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "ls";
    json_to_sent["payload"]["dir_name"] = dir_name;
    return json_to_sent;
}
/*
void nar::MessageTypes::IPCLs::Request::send__action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["_action"] = "config";
    json_to_sent["payload"]["dir_name"] = dir_name;
    send_message(skt, json_to_sent.dump());
    return;
}
*/
nlohmann::json nar::MessageTypes::IPCLs::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
