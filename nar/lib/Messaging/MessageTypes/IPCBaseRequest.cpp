#include "IPCBaseRequest.h"

nlohmann::json nar::MessageTypes::IPCBaseRequest::generate_json() {
    nlohmann::json jsn;
    try{
        jsn["header"]["action"] = this->_action;
        jsn["header"]["username"] = this->_username;
        jsn["header"]["password"] = this->_password;
        jsn["header"]["current_directory"] = this->_current_directory;
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, we couldn't generate json");
    }
    return jsn;
}

void nar::MessageTypes::IPCBaseRequest::populate_object(nlohmann::json& jsn) {
    try{
        this->_action = jsn["header"]["action"];
        this->_username = jsn["header"]["username"];
        this->_password = jsn["header"]["password"];
        this->_current_directory = jsn["header"]["current_directory"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, we couldn't populate the object");
    }
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
    try{
        header["action"] = _action;
        header["username"] = this->_username;
        header["password"] = this->_password;
        header["current_directory"] = this->_current_directory;
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, we couldn't fill the head");
    }
    return header;
}

void nar::MessageTypes::IPCBaseRequest::recvThe_action(nlohmann::json &recv){
    try{
        this -> _action = recv["action"];
        this-> _username = recv["username"];
        this-> _password = recv["password"];
        this-> _current_directory = recv["current_directory"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, we couldn't receive the action");
    }
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseRequest::get_myrequestjson() {
    nlohmann::json my_request_json;
    try{
        my_request_json["header"]["reply_to"] = _action;
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, we couldn't get the request json");
    }
    return my_request_json;
}

void nar::MessageTypes::IPCBaseRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    try{
        json_to_sent["header"]["reply_to"] = _action;
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest("in IPCBaseRequest, there is a bad json constructed in send action");
    }
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCBaseRequest::print_loop(nar::Socket* skt) {
    bool isError = true;
    while(true){
        bool flag = false;
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(tmp);
        std::cout << tmp << std::endl;
        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
        int statcode = received["header"]["status_code"];
        statcode /= 100;
        switch(statcode) {
            case 3:
                throw nar::Exception::MessageTypes::InternalServerError("There is problem in the request that came to server", statcode);
            case 4:
                throw nar::Exception::MessageTypes::InternalServerDatabaseError("There is a problem in database side of the server", statcode);
            case 5:
                throw nar::Exception::MessageTypes::InternalServerError("There is a problem in the server", statcode);
            case 6:
                throw nar::Exception::MessageTypes::InternalDaemonError("There is a problem in the daemon", statcode);
            case 7:
                throw nar::Exception::MessageTypes::InternalCliError("There is a problem in the cli", statcode);
            default:
                flag = true;
        }
        if(flag) {
            break;
        }
    }
    return;
}
