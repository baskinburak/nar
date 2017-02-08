#include "KeepAlive.h"

void nar::Messagetypes::KeepAlive::Request::send_mess(nar::Socket* skt) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = sendHead();
    send_message(skt,keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    receive_message(keep_req_recv);
    return;
}
void nar::Messagetypes::KeepAlive::Request::receive_message(nlohmann::json keep_req_recv){
    nlohmann::json head = keep_req_recv["header"];
    recvFill(head);
    return;
}
nlohmann::json nar::Messagetypes::KeepAlive::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = sendHead();
    return keep_req_test;
}

void nar::Messagetypes::KeepAlive::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = sendHead();
    send_message(skt,keep_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_resp_recv = nlohmann::json::parse(temp);
    receive_message(keep_resp_recv);
    return;
}
void nar::Messagetypes::KeepAlive::Response::receive_message(nlohmann::json keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recvFill(head);
    return;
}

nlohmann::json nar::Messagetypes::KeepAlive::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = sendHead();
    return keep_resp_test;
}
