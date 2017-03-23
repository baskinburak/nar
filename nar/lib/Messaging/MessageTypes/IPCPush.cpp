#include "IPCPush.h"

nlohmann::json nar::MessageTypes::IPCPush::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "push";
    json_to_sent["payload"]["file_name"] = file_name;
    return json_to_sent;
}

void nar::MessageTypes::IPCPush::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "push";
    json_to_sent["payload"]["file_name"] = file_name;
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCPush::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["process_name"] = get_process_name();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
