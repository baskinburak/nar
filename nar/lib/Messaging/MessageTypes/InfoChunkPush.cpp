#include "InfoChunkPush.h"

long long int nar::messagetypes::InfoChunkPush::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::messagetypes::InfoChunkPush::Request::get_success() {
    return success;
}

void nar::messagetypes::InfoChunkPush::Request::sendMessage() {
    nlohmann::json ipush_req_send;
    ipush_req_send["header"] = sendHead();
    ipush_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipush_req_send["payload"]["success"] = this->success;
    std::cout<<ipush_req_send.dump()<<std::endl;
    return;

}
void nar::messagetypes::InfoChunkPush::Request::receiveMessage(nlohmann::json ipush_req_recv) {
    nlohmann::json head = ipush_req_recv["header"];
    recvFill(head);
    this->chunk_id = ipush_req_recv["payload"]["chunk_id"];
    this->success = ipush_req_recv["payload"]["success"];
    return;
}

void nar::messagetypes::InfoChunkPush::Response::sendMessage() {
    nlohmann::json ipush_resp_send;
    ipush_resp_send["header"] = sendHead();
    std::cout<<ipush_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::InfoChunkPush::Response::receiveMessage(nlohmann::json ipush_resp_recv) {
    nlohmann::json head = ipush_resp_recv["header"];
    recvFill(head);
    return;
}
