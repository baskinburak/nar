#include "IPCStatus.h"

nlohmann::json nar::MessageTypes::IPCStatus::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "status";
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCStatus::Request::generate_json() {
	nlohmann::json jsn = IPCBaseRequest::generate_json();
    return jsn;
}
void nar::MessageTypes::IPCStatus::Request::populate_object(std::string& jsn_str) {
	auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
}

void nar::MessageTypes::IPCStatus::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCStatus::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
