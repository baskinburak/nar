#include "Handshake.h"

std::string& nar::MessageTypes::Handshake::Request::get_username() {
    return username;
}
std::string& nar::MessageTypes::Handshake::Request::get_machine_id() {
    return machine_id;
}
void nar::MessageTypes::Handshake::Request::send_mess(nar::Socket* skt) {
    nlohmann::json hand_req_send;
    hand_req_send["header"] = send_head();
    hand_req_send["payload"]["username"] = this->username;
    hand_req_send["payload"]["machine_id"] = this->machine_id;
    send_message(skt,hand_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json hand_req_recv = nlohmann::json::parse(temp);
    receive_message(hand_req_recv);
    return;
}
void nar::MessageTypes::Handshake::Request::receive_message(nlohmann::json hand_req_recv){
    nlohmann::json head = hand_req_recv["header"];
    recv_fill(head);
    this->username = hand_req_recv["payload"]["username"];
    this->machine_id = hand_req_recv["payload"]["machine_id"];
    return;
}
void nar::MessageTypes::Handshake::Request::receive_message(std::string str){
    nlohmann::json hand_req_recv = nlohmann::json::parse(str);
    nlohmann::json head = hand_req_recv["header"];
    recv_fill(head);
    this->username = hand_req_recv["payload"]["username"];
    this->machine_id = hand_req_recv["payload"]["machine_id"];
    return;
}
nlohmann::json nar::MessageTypes::Handshake::Request::test_json() {
    nlohmann::json hand_req_test;
    hand_req_test["header"] = send_head();
    hand_req_test["payload"]["username"] = this->username;
    hand_req_test["payload"]["machine_id"] = this->machine_id;
    return hand_req_test;
}
void nar::MessageTypes::Handshake::Response::send_mess(nar::Socket* skt) {
    nlohmann::json hand_resp_send;
    hand_resp_send["header"] = send_head();
    send_message(skt,hand_resp_send.dump());
    return;
}
void nar::MessageTypes::Handshake::Response::receive_message(nlohmann::json hand_resp_recv){
    nlohmann::json head = hand_resp_recv["header"];
    recv_fill(head);
    return;
}

nlohmann::json nar::MessageTypes::Handshake::Response::test_json() {
    nlohmann::json hand_resp_test;
    hand_resp_test["header"] = send_head();
    return hand_resp_test;
}
