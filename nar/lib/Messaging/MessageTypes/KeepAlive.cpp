#include "KeepAlive.h"

void nar::messagetypes::KeepAlive::Request::sendMessage() {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = sendHead();
    std::cout<<keep_req_send.dump()<<std::endl;
    return;
}
void nar::messagetypes::KeepAlive::Request::receiveMessage(nlohmann::json keep_req_recv){
    nlohmann::json head = keep_req_recv["header"];
    recvFill(head);
    return;
}
void nar::messagetypes::KeepAlive::Response::sendMessage() {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = sendHead();
    std::cout<<keep_resp_send.dump()<<std::endl;
    return;
}
void nar::messagetypes::KeepAlive::Response::receiveMessage(nlohmann::json keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recvFill(head);
    return;
}
