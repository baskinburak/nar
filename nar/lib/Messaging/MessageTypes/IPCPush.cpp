#include "IPCPush.h"

std::string nar::MessageTypes::IPCPush::Request::get_file_path(){
    return _file_path;
}

void nar::MessageTypes::IPCPush::Request::set_file_path(std::string fp){
    this -> _file_path = fp;
}

nlohmann::json nar::MessageTypes::IPCPush::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "push";
    json_to_sent["payload"]["file_path"] = _file_path;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCPush::Request::generate_json() {
	nlohmann::json jsn = IPCBaseRequest::generate_json();
	jsn["payload"]["file_path"] = this->_file_path;
    return jsn;	
}
void nar::MessageTypes::IPCPush::Request::populate_object(std::string& jsn_str) {
    try {
    	auto jsn = nlohmann::json::parse(jsn_str);
        IPCBaseRequest::populate_object(jsn);
        this->_file_path = jsn["payload"]["file_path"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("IPCPush bad message received");
    }
}

void nar::MessageTypes::IPCPush::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCPush::Request::receive_message(nlohmann::json &js){
	//bunu sil    
	//this -> _action = js["header"]["action"];
    this -> _file_path = js["payload"]["file_path"];
    return;
}

nlohmann::json nar::MessageTypes::IPCPush::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
