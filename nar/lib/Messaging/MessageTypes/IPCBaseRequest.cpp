#include "IPCBaseRequest.h"

nlohmann::json nar::MessageTypes::IPCBaseRequest::generate_json() {
    nlohmann::json jsn;
    jsn["header"]["action"] = this->_action;
    jsn["header"]["username"] = this->_username;
    jsn["header"]["password"] = this->_password;
    jsn["header"]["current_directory"] = this->_current_directory;
    return jsn;
}

void nar::MessageTypes::IPCBaseRequest::populate_object(nlohmann::json& jsn) {
    this->_action = jsn["header"]["action"];
    this->_username = jsn["header"]["username"];
    this->_password = jsn["header"]["password"];
    this->_current_directory = jsn["header"]["current_directory"];
}

std::string nar::MessageTypes::IPCBaseRequest::get_action() {
    return _action;
}

void nar::MessageTypes::IPCBaseRequest::set_action(std::string an) {
    _action = an;
    return;
}

std::string& nar::MessageTypes::IPCBaseRequest::get_username() {
    return _username;
}

std::string& nar::MessageTypes::IPCBaseRequest::get_current_directory() {
    return _current_directory;
}

std::string& nar::MessageTypes::IPCBaseRequest::get_password() {
    return _password;
}

void nar::MessageTypes::IPCBaseRequest::set_username(std::string& un) {
    _username = un;
    return;
}

void nar::MessageTypes::IPCBaseRequest::set_current_directory(std::string& cd) {
    _current_directory = cd;
    return;
}

void nar::MessageTypes::IPCBaseRequest::set_password(std::string& pw) {
    _password = pw;
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
    this-> _username = recv["username"];
    this-> _password = recv["password"];
    this-> _current_directory = recv["current_directory"];
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
    bool isError = true;
    while(true){
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(tmp);
        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
        switch(stat_code) {
            case stat_code / 100 == 3:
                std::cout << "There is a problem in the request that came to server" << std::endl;
                break;
            case stat_code / 100 == 4:
                std::cout << "There is problem in database of the server" << std::endl;
                break;
            case stat_code / 100 == 5:
                std::cout << "There is problem in the server" << std::endl;
                break
            case stat_code / 100 == 6:
                std::cout << "There are internal daemon errors" << std::endl;
                break
            default:
                flag = false;
        }
        if(flag) {
            break;
        }
    }
    return;
}
