#include "KeepAlive.h"

void nar::MessageTypes::KeepAlive::Request::send_mess(nar::Socket* skt, nar::MessageTypes::KeepAlive::Response & resp) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    keep_req_send["payload"]["machine_id"] = this->_machine_id;
    send_message(skt,keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(keep_req_recv);
    return;
}
void nar::MessageTypes::KeepAlive::Request::receive_message(nlohmann::json keep_req_recv){
    nlohmann::json head = keep_req_recv["header"];
    recv_fill(head);
    this->_machine_id = keep_req_send["payload"]["machine_id"];
    return;
}
nlohmann::json nar::MessageTypes::KeepAlive::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::KeepAlive::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::KeepAlive::Response::receive_message(nlohmann::json& keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recv_fill(head);

    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::ServerSocketAuthenticationError("Server can not authenticate socket created for this user", _status_code);
    }
    return;
}

nlohmann::json nar::MessageTypes::KeepAlive::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}
