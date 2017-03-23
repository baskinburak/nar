#include "IPCPull.h"

nlohmann::json nar::MessageTypes::IPCPull::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "pull";
    json_to_sent["payload"]["file_name"] = file_name;
    json_to_sent["payload"]["cur_dir"] = cur_dir;
    return json_to_sent;
}

void nar::MessageTypes::IPCPull::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "pull";
    json_to_sent["payload"]["file_name"] = file_name;
    json_to_sent["payload"]["cur_dir"] = cur_dir;
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCPull::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["process_name"] = get_process_name();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
