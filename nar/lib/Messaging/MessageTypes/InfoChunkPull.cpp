#include "InfoChunkPull.h"

long long int nar::messagetypes::InfoChunkPull::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::messagetypes::InfoChunkPull::Request::get_success() {
    return success;
}

void nar::messagetypes::InfoChunkPull::Request::sendMessage() {
    nlohmann::json ipull_req_send;
    ipull_req_send["header"] = sendHead();
    ipull_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_send["payload"]["success"] = this->success;
    std::cout<<ipull_req_send.dump()<<std::endl;
    return;

}
void nar::messagetypes::InfoChunkPull::Request::receiveMessage(nlohmann::json ipull_req_recv) {
    nlohmann::json head = ipull_req_recv["header"];
    recvFill(head);
    this->chunk_id = ipull_req_recv["payload"]["chunk_id"];
    this->success = ipull_req_recv["payload"]["success"];
    return;
}

void nar::messagetypes::InfoChunkPull::Response::sendMessage() {
    nlohmann::json ipull_resp_send;
    ipull_resp_send["header"] = sendHead();
    std::cout<<ipull_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::InfoChunkPull::Response::receiveMessage(nlohmann::json ipull_resp_recv) {
    nlohmann::json head = ipull_resp_recv["header"];
    recvFill(head);
    return;
}
