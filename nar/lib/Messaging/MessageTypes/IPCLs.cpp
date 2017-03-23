#include "IPCLs.h"

nlohmann::json nar::MessageTypes::IPCLs::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "ls";
    json_to_sent["payload"]["dir_name"] = dir_name;
    return json_to_sent;
}

void nar::MessageTypes::IPCLs::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "config";
    json_to_sent["payload"]["dir_name"] = dir_name;
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCLs::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["process_name"] = get_process_name();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
