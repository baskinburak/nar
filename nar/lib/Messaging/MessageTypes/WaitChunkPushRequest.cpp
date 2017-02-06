#include "WaitChunkPushRequest.h"
std::string& nar::messagetypes::WaitChunkPushRequest::Request::get_stream_id() {
    return stream_id;
}
unsigned short nar::messagetypes::WaitChunkPushRequest::Request::get_randevous_port() {
    return rand_port;
}
long long int nar::messagetypes::WaitChunkPushRequest::Request::get_chunk_id() {
    return chunk_id;
}

long long int  nar::messagetypes::WaitChunkPushRequest::Request::get_chunk_size() {
    return chunk_size;
}
void nar::messagetypes::WaitChunkPushRequest::Request::sendMessage() {
    nlohmann::json wpush_req_send;
    wpush_req_send["header"] = sendHead();
    wpush_req_send["payload"]["stream_id"] = this->stream_id;
    wpush_req_send["payload"]["rand_port"] = this->rand_port;
    wpush_req_send["payload"]["chunk_size"] =  this->chunk_size;
    wpush_req_send["payload"]["chunk_id"] = this->chunk_id;
    std::cout<<wpush_req_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::WaitChunkPushRequest::Request::receiveMessage(nlohmann::json wpush_req_recv) {
    nlohmann::json head = wpush_req_recv["header"];
    recvFill(head);
    this->stream_id = wpush_req_recv["payload"]["stream_id"];
    this->rand_port = wpush_req_recv["payload"]["rand_port"];
    this->chunk_size = wpush_req_recv["payload"]["chunk_size"];
    this->chunk_id = wpush_req_recv["payload"]["chunk_id"];
    return;
}

void nar::messagetypes::WaitChunkPushRequest::Response::sendMessage() {
    nlohmann::json wpush_resp_send;
    wpush_resp_send["header"] = sendHead();
    std::cout<<wpush_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::WaitChunkPushRequest::Response::receiveMessage(nlohmann::json wpush_resp_recv) {
    nlohmann::json head = wpush_resp_recv["header"];
    recvFill(head);
    return;
}
