#include "Handshake.h"

std::string& nar::messagetypes::Handshake::Request::getUsername() {
    return username;
}
std::string& nar::messagetypes::Handshake::Request::getMachineId() {
    return machine_id;
}
void nar::messagetypes::Handshake::Request::sendMessage() {
    nlohmann::json hand_req_send;
    hand_req_send["header"] = sendHead();
    hand_req_send["payload"]["username"] = this->username;
    hand_req_send["payload"]["machine_id"] = this->machine_id;
    std::cout<<hand_req_send.dump()<<std::endl;
    return;
}
void nar::messagetypes::Handshake::Request::receiveMessage(nlohmann::json hand_req_recv){
    nlohmann::json head = hand_req_recv["header"];
    recvFill(head);
    this->username = hand_req_recv["payload"]["username"];
    this->machine_id = hand_req_recv["payload"]["machine_id"];
    return;
}
void nar::messagetypes::Handshake::Response::sendMessage() {
    nlohmann::json hand_resp_send;
    hand_resp_send["header"] = sendHead();
    std::cout<<hand_resp_send<<std::endl;
    return;
}
void nar::messagetypes::Handshake::Response::receiveMessage(nlohmann::json hand_resp_recv){
    nlohmann::json head = hand_resp_recv["header"];
    recvFill(head);
    return;
}
