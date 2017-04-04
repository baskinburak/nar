#include "IPCRegister.h"

nlohmann::json nar::MessageTypes::IPCRegister::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "register";
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCRegister::Request::generate_json() {
	nlohmann::json jsn = IPCBaseRequest::generate_json();
    return jsn;	
}
void nar::MessageTypes::IPCRegister::Request::populate_object(std::string& jsn_str) {
	auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
}

void nar::MessageTypes::IPCRegister::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCRegister::Request::receive_message(nlohmann::json &js){
	//bunu silcemm    
	//this -> _action = js["header"]["action"];
    return;
}

nlohmann::json nar::MessageTypes::IPCRegister::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
