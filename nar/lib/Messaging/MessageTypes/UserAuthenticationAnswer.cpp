#include "UserAuthenticationAnswer.h"

void nar::MessageTypes::UserAuthenticationAnswer::Request::send_mess(nar::Socket* skt, nar::MessageTypes::UserAuthenticationAnswer::Response & resp) {
    nlohmann::json uans_req_send;
    uans_req_send["header"] = send_head();
    uans_req_send["payload"]["result_string"] = this->_result_string;

    try {    
        send_message(skt,uans_req_send.dump());
    } catch(nar::Exception::LowLevelMessaging::NoSize& exp) {
        throw nar::Exception::LowLevelMessaging::Error("LowLevelMessaging::NoSize error");
    } catch( nar::Exception::LowLevelMessaging::FormatError& exp) {
        throw nar::Exception::LowLevelMessaging::Error("LowLevelMessaging::FormatError");
    } catch(nar::Exception::LowLevelMessaging::ServerSizeIntOverflow& exp) {
        throw nar::Exception::LowLevelMessaging::Error("LowLevelMessaging::ServerSizeIntOverflow");
    }
    
    std::string temp;

    try {
        temp = get_message(skt);
    } catch(nar::Exception::LowLevelMessaging::NoSize& exp) {
        throw nar::Exception::LowLevelMessaging::Error("LowLevelMessaging::NoSize error");
    } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow& exp) {
        throw nar::Exception::LowLevelMessaging::Error("LowLevelMessaging::SizeIntOverflow error");
    }

    nlohmann::json uans_req_recv;

    try {
        uans_req_recv = nlohmann::json::parse(temp);
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("UserAuthenticationAnswer::Request::send_mess");
    }

    resp.receive_message(uans_req_recv);
    return;
}
void nar::MessageTypes::UserAuthenticationAnswer::Request::receive_message(std::string& recv_req){
    auto uans_req_recv = nlohmann::json::parse(recv_req);
    try {
        nlohmann::json head = uans_req_recv["header"];
        recv_fill(head);
        this->_result_string = uans_req_recv["payload"]["result_string"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("user authentication answer error");
    }

    if(this->_result_string.empty()) {
        throw nar::Exception::MessageTypes::BadMessageReceive("user authentication answer result string cant be empty");
    }

    return;
}
nlohmann::json nar::MessageTypes::UserAuthenticationAnswer::Request::test_json() {
    nlohmann::json uans_req_recv;
    uans_req_recv["header"] = send_head();
    uans_req_recv["payload"]["result_string"] = this->_result_string;
    return uans_req_recv;
}

void nar::MessageTypes::UserAuthenticationAnswer::Response::send_mess(nar::Socket* skt) {
    nlohmann::json uans_resp_send;
    uans_resp_send["header"] = send_head();
    send_message(skt,uans_resp_send.dump());
    return;
}
void nar::MessageTypes::UserAuthenticationAnswer::Response::receive_message(nlohmann::json uans_resp_recv){
    try {
        nlohmann::json head = uans_resp_recv["header"];
        recv_fill(head);
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("UserAuthenticationAnswer::Response::receive_message");

    }

    int stat = get_status_code();

    if(stat == 703) {
        throw nar::Exception::Authentication::WrongTaskString("UserAuthenticationAnswer::Response::receive_message");
    } else if(stat == 200) {
    } else {
        throw nar::Exception::MessageTypes::BadMessageReceive("UserAuthenticationInit::Response::receive_message UnknownStatCode");
    }

    return;
}

nlohmann::json nar::MessageTypes::UserAuthenticationAnswer::Response::test_json() {
    nlohmann::json keep_resp_test;
    return keep_resp_test;
}

std::string& nar::MessageTypes::UserAuthenticationAnswer::Request::get_result_string() {
    return _result_string;
}
