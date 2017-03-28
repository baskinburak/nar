#include "IPCStatus.h"

nlohmann::json nar::MessageTypes::IPCStatus::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "status";
    return json_to_sent;
}
/*
void nar::MessageTypes::IPCStatus::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "status";
    send_message(skt, json_to_sent.dump());
    return;
}
*/

nlohmann::json nar::MessageTypes::IPCStatus::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["process_name"] = get_process_name();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}