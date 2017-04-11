#include "IPCPull.h"

std::string nar::MessageTypes::IPCPull::Request::get_file_name(){
    return _file_name;
}

void nar::MessageTypes::IPCPull::Request::set_file_name(std::string fn){
    this -> _file_name = fn;
}

nlohmann::json nar::MessageTypes::IPCPull::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "pull";
    json_to_sent["payload"]["file_name"] = _file_name;
    json_to_sent["payload"]["dir_name"] = _dir_name;
    return json_to_sent;
}
std::string nar::MessageTypes::IPCPull::Request::get_dir_path() {
    return _dir_name;
}
nlohmann::json nar::MessageTypes::IPCPull::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
	jsn["payload"]["file_name"] = this->_file_name;
    jsn["payload"]["dir_name"] = this->_dir_name;
    return jsn;
}

void nar::MessageTypes::IPCPull::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_file_name = jsn["payload"]["file_name"];
    this->_dir_name = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCPull::Request::send_action(nar::Socket* skt) {
	nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCPull::Request::receive_message(nlohmann::json &js){
	//sil bu func i    
	//this -> _action = js["header"]["action"];
    this -> _file_name = js["payload"]["file_name"];
    this->_dir_name = js["payload"]["dir_name"];
    return;
}

nlohmann::json nar::MessageTypes::IPCPull::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
    return resp_json;
}
