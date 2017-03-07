#include "WaitChunkPull.h"
long long int nar::MessageTypes::WaitChunkPull::Request::get_stream_id() {
    return stream_id;
}
unsigned short nar::MessageTypes::WaitChunkPull::Request::get_randevous_port() {
    return rand_port;
}
long long int nar::MessageTypes::WaitChunkPull::Request::get_chunk_id() {
    return chunk_id;
}

long long int  nar::MessageTypes::WaitChunkPull::Request::get_chunk_size() {
    return chunk_size;
}
void nar::MessageTypes::WaitChunkPull::Request::send_mess(nar::Socket* skt,nar::MessageTypes::WaitChunkPull::Response & resp) {
    nlohmann::json wpull_req_send;
    wpull_req_send["header"] = sendHead();
    wpull_req_send["payload"]["stream_id"] = this->stream_id;
    wpull_req_send["payload"]["rand_port"] = this->rand_port;
    wpull_req_send["payload"]["chunk_size"] =  this->chunk_size;
    wpull_req_send["payload"]["chunk_id"] = this->chunk_id;
    send_message(skt,wpull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json wpull_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(wpull_req_recv);
    return;
}
void nar::MessageTypes::WaitChunkPull::Request::receive_message(nlohmann::json wpull_req_recv) {
    nlohmann::json head = wpull_req_recv["header"];
    recvFill(head);
    this->stream_id = wpull_req_recv["payload"]["stream_id"];
    this->rand_port = wpull_req_recv["payload"]["rand_port"];
    this->chunk_size = wpull_req_recv["payload"]["chunk_size"];
    this->chunk_id = wpull_req_recv["payload"]["chunk_id"];
    return;
}
nlohmann::json nar::MessageTypes::WaitChunkPull::Request::test_json() {
    nlohmann::json wpull_req_test;
    wpull_req_test["header"] = sendHead();
    wpull_req_test["payload"]["stream_id"] = this->stream_id;
    wpull_req_test["payload"]["rand_port"] = this->rand_port;
    wpull_req_test["payload"]["chunk_size"] =  this->chunk_size;
    wpull_req_test["payload"]["chunk_id"] = this->chunk_id;
    return wpull_req_test;
}


void nar::MessageTypes::WaitChunkPull::Response::send_mess(nar::Socket* skt) {
    nlohmann::json wpull_resp_send;
    wpull_resp_send["header"] = sendHead();
    send_message(skt,wpull_resp_send.dump());
    return;
}

void nar::MessageTypes::WaitChunkPull::Response::receive_message(nlohmann::json wpull_resp_recv) {
    nlohmann::json head = wpull_resp_recv["header"];
    recvFill(head);
    return;
}
nlohmann::json nar::MessageTypes::WaitChunkPull::Response::test_json() {
    nlohmann::json wpull_resp_test;
    wpull_resp_test["header"] = sendHead();
    return wpull_resp_test;
}
