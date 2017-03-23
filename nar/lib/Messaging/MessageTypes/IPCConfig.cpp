#include "IPCConfig.h"

nlohmann::json nar::MessageTypes::IPCConfig::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "config";
    json_to_sent["payload"]["var"] = _var;
    json_to_sent["payload"]["value"] = _value;
    return json_to_sent;
}

void nar::MessageTypes::IPCConfig::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action_name"] = "config";
    json_to_sent["payload"]["var"] = _var;
    json_to_sent["payload"]["value"] = _value;
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCConfig::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["process_name"] = get_process_name();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
