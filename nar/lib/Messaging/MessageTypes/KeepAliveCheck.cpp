#include "KeepAliveCheck.h"

void nar::MessageTypes::KeepAliveCheck::Request::send_mess(nar::Socket* skt, nar::MessageTypes::KeepAliveCheck::Response & resp) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    send_message(skt,keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(keep_req_recv);
    return;
}
void nar::MessageTypes::KeepAliveCheck::Request::receive_message(std::string& msg){
    auto keep_req_recv = nlohmann::json::parse(msg);
    nlohmann::json head = keep_req_recv["header"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::KeepAliveCheck::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::KeepAliveCheck::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::KeepAliveCheck::Response::receive_message(nlohmann::json& keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recv_fill(head);
    return;
}

nlohmann::json nar::MessageTypes::KeepAliveCheck::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}

