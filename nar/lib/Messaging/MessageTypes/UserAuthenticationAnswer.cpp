#include "UserAuthenticationAnswer.h"

void nar::MessageTypes::UserAuthenticationAnswer::Request::send_mess(nar::Socket* skt, nar::MessageTypes::UserAuthenticationAnswer::Response & resp) {
    nlohmann::json uans_req_send;
    uans_req_send["header"] = send_head();
    uans_req_send["payload"]["result_string"] = this->_result_string;
    send_message(skt,uans_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json uans_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(uans_req_recv);
    return;
}
void nar::MessageTypes::UserAuthenticationAnswer::Request::receive_message(nlohmann::json uans_req_recv){
    nlohmann::json head = uans_req_recv["header"];
    this->_result_string = uans_req_recv["payload"]["result_string"];
    recv_fill(head);
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
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::UserAuthenticationAnswer::Response::receive_message(nlohmann::json uans_resp_recv){
    nlohmann::json head = uans_resp_recv["header"];
    recv_fill(head);
    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::ResultStringIsWrong("Result string is wrong", _status_code);
    } 
    return;
}

nlohmann::json nar::MessageTypes::UserAuthenticationAnswer::Response::test_json() {
    nlohmann::json keep_resp_test;
    return keep_resp_test;
}

std::string& nar::MessageTypes::UserAuthenticationAnswer::Request::get_result_string() {
    return _username;
}
