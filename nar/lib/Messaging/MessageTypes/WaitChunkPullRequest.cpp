#include "WaitChunkPullRequest.h"
std::string& nar::messagetypes::WaitChunkPullRequest::Request::get_stream_id() {
    return stream_id;
}
unsigned short nar::messagetypes::WaitChunkPullRequest::Request::get_randevous_port() {
    return rand_port;
}
long long int nar::messagetypes::WaitChunkPullRequest::Request::get_chunk_id() {
    return chunk_id;
}

long long int  nar::messagetypes::WaitChunkPullRequest::Request::get_chunk_size() {
    return chunk_size;
}
void nar::messagetypes::WaitChunkPullRequest::Request::sendMessage() {
    nlohmann::json wpull_req_send;
    wpull_req_send["header"] = sendHead();
    wpull_req_send["payload"]["stream_id"] = this->stream_id;
    wpull_req_send["payload"]["rand_port"] = this->rand_port;
    wpull_req_send["payload"]["chunk_size"] =  this->chunk_size;
    wpull_req_send["payload"]["chunk_id"] = this->chunk_id;
    std::cout<<wpull_req_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::WaitChunkPullRequest::Request::receiveMessage(nlohmann::json wpull_req_recv) {
    nlohmann::json head = wpull_req_recv["header"];
    recvFill(head);
    this->stream_id = wpull_req_recv["payload"]["stream_id"];
    this->rand_port = wpull_req_recv["payload"]["rand_port"];
    this->chunk_size = wpull_req_recv["payload"]["chunk_size"];
    this->chunk_id = wpull_req_recv["payload"]["chunk_id"];
    return;
}

void nar::messagetypes::WaitChunkPullRequest::Response::sendMessage() {
    nlohmann::json wpull_resp_send;
    wpull_resp_send["header"] = sendHead();
    std::cout<<wpull_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::WaitChunkPullRequest::Response::receiveMessage(nlohmann::json wpull_resp_recv) {
    nlohmann::json head = wpull_resp_recv["header"];
    recvFill(head);
    return;
}
