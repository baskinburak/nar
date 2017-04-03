#include "IPCBaseRequest.h"

std::string nar::MessageTypes::IPCBaseRequest::get_action() {
    return _action;
}

void nar::MessageTypes::IPCBaseRequest::set_action(std::string an) {
    _action = an;
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseRequest::fillTheHead() {
    nlohmann::json header;
    header["action"] = _action;
    header["username"] = this->_username;
    header["password"] = this->_password;
    header["current_directory"] = this->_current_directory;
    return header;
}

void nar::MessageTypes::IPCBaseRequest::recvThe_action(nlohmann::json &recv){
    this -> _action = recv["action"];
    this->_username = recv["username"];
    this->_password = recv["password"];
    this->_current_directory = recv["current_directory"];
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseRequest::get_myrequestjson() {
    nlohmann::json my_request_json;
    my_request_json["header"]["reply_to"] = _action;
    return my_request_json;
}

void nar::MessageTypes::IPCBaseRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["reply_to"] = _action;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCBaseRequest::print_loop(nar::Socket* skt) {
    while(true){
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(temp);
        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
    }
    return;
}
