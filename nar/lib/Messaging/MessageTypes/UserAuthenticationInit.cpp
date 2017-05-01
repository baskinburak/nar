#include "UserAuthenticationInit.h"


nar::MessageTypes::UserAuthenticationInit::Response::Response() {
    ResponseHeader(-1, std::string("user_authentication_init"));
    this->_private_key = "";
    this->_task_string = "";
    this->_aes_crypted = "";
}
nar::MessageTypes::UserAuthenticationInit::Response::Response(int status_code) {
    ResponseHeader(status_code, std::string("user_authentication_init"));
    this->_private_key = "";
    this->_task_string = "";
    this->_aes_crypted = "";
}

void nar::MessageTypes::UserAuthenticationInit::Request::send_mess(nar::Socket* skt, nar::MessageTypes::UserAuthenticationInit::Response & resp) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    keep_req_send["payload"]["username"] = this->_username;
    send_message(skt,keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(keep_req_recv);
    return;
}
void nar::MessageTypes::UserAuthenticationInit::Request::receive_message(std::string& recv_msg){
    auto keep_req_recv = nlohmann::json::parse(recv_msg);
    try {
        nlohmann::json head = keep_req_recv["header"];
        recv_fill(head);
        this->_username = keep_req_recv["payload"]["username"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("user authentication init request bad json");
    }

    return;
}
nlohmann::json nar::MessageTypes::UserAuthenticationInit::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::UserAuthenticationInit::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    if(get_status_code() != 200) {
        send_message(skt,keep_resp_send.dump());
        return;
    }

    if((this->_private_key=="") || (this->_task_string=="") || (this->_aes_crypted =="")) {
        throw nar::Exception::MessageTypes::BadlyConstructedMessageSend("UserAuthenticationInit can not send in this state");
    }
    keep_resp_send["payload"]["private_key"] = _private_key;
    keep_resp_send["payload"]["task_string"] = _task_string;
    keep_resp_send["payload"]["aes_crypted"] = _aes_crypted;
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::UserAuthenticationInit::Response::receive_message(nlohmann::json keep_resp_recv){

    try {
        nlohmann::json head = keep_resp_recv["header"];
        recv_fill(head);
    }
    catch(nar::Exception::MessageTypes::ResponseRecvFillError exp) {
        std::cout<<exp.what()<<std::endl;
        throw nar::Exception::MessageTypes::BadMessageReceive(exp.what().c_str());

    }
    int stat = get_status_code();
    if(stat/100 == 3) {
        throw nar::Exception::MessageTypes::BadRequest("Your request was not complete or was wrong", _status_code);
    } else  if(stat/100 == 4) {
        throw nar::Exception::MessageTypes::InternalServerDatabaseError("Database insertion problem", _status_code);
    } else  if(stat/100 == 5) {
        throw nar::Exception::MessageTypes::InternalServerError("Some thing went wrong in server", _status_code);
    }
    try {
        this->_private_key = keep_resp_recv["payload"]["private_key"];
        this->_task_string = keep_resp_recv["payload"]["task_string"];
        this->_aes_crypted = keep_resp_recv["payload"]["aes_crypted"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("user authentication init response bad");
    }
    if(this->_private_key.empty() || this->_task_string.empty() || this->_aes_crypted.empty()) {
        throw nar::Exception::MessageTypes::BadMessageReceive("user authentication items cant be empty");
    }
    return;
}

nlohmann::json nar::MessageTypes::UserAuthenticationInit::Response::test_json() {
    nlohmann::json keep_resp_test;
    return keep_resp_test;
}

std::string & nar::MessageTypes::UserAuthenticationInit::Response::get_private_key() {
    return _private_key;
}

std::string & nar::MessageTypes::UserAuthenticationInit::Response::get_task_string() {
    return _task_string;
}

std::string& nar::MessageTypes::UserAuthenticationInit::Request::get_username() {
    return _username;
}

std::string& nar::MessageTypes::UserAuthenticationInit::Response::get_aes_crypted() {
    return _aes_crypted;
}
