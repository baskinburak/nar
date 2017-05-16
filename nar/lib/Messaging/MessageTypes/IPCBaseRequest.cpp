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
    try{
        this-> _action = recv["action"];
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
        my_request_json["header"]["reply_to"] = _action;
    return my_request_json;
}

void nar::MessageTypes::IPCBaseRequest::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["reply_to"] = _action;
    try {
        send_message(skt, json_to_sent.dump());
    }
    catch (nar::Exception::ExcpBase& e) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemRequest(std::string( "in IPCBaseRequest:: " ).append(e.what()));
    }
    return;
}

void nar::MessageTypes::IPCBaseRequest::print_loop(nar::Socket* skt) {
    while(true) {
        std::string tmp;
        try {
            tmp = get_message(*skt);
        } catch(nar::Exception::Socket::SystemError& Exp) {
            throw;
        } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow& Exp) {
            throw nar::Exception::LowLevelMessaging::Error("Low level messaging error in IPCPush::send_action.");
        } catch(nar::Exception::LowLevelMessaging::FormatError& Exp) {
            throw nar::Exception::LowLevelMessaging::Error("Low level messaging error in IPCPush::send_action.");
        } catch(...) {
            throw;
        }


        nlohmann::json received;
        try {
            received = nlohmann::json::parse(tmp);
        } catch(...) {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message not JSON received in print_loop");
        }
        int statcode;
        try {
            if(received["header"]["reply_to"].get<std::string>() == std::string("END")) {
                break;
            }
            statcode = received["payload"]["status_code"];
        }
        catch ( ... ) {
            throw nar::Exception::MessageTypes::BadMessageReceive("Bad message received in print_loop");
        }


        if(statcode == 200) {
            std::cout << "SUCCESS" << std::endl;
        } else if(statcode == 601) {
            std::cout << "Cannot connect to the server." << std::endl;
        } else if(statcode == 602) {
            std::cout << "Low level messaging error with server." << std::endl;
        } else if(statcode == 603) {
            std::cout << "Server sent bad message. Are you connected to a legit server?" << std::endl << "Check your config file." << std::endl;
        } else if(statcode == 702) {
            std::cout << "Cannot authenticate your user" << std::endl;
        } else if(statcode == 900){
            std::cout << "Unknown error." << std::endl;
        } else if(statcode == 501) {
            std::cout << "AesCryptor unexpected server error." << std::endl;
        } else if(statcode == 502) {
            std::cout << "RsaCryptor unexpected server error." << std::endl;
        } else if(statcode == 503) {
            std::cout << "base64 unexpected server error." << std::endl;
        } else if(statcode == 704) {
            std::cout << "Cannot open file." << std::endl;
        } else if(statcode == 705) {
            std::cout << "Cannot open a tempfile." << std::endl;
        } else if(statcode == 706) {
            std::cout << "Compression error." << std::endl;
        } else if(statcode == 707) {
            std::cout << "Cryption error." << std::endl;
        } else {
            std::cout << "Non-handled error." << std::endl;
        }

    }

    skt->close();
}
