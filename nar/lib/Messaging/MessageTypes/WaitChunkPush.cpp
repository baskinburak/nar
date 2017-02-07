#include "WaitChunkPush.h"
std::string& nar::Messagetypes::WaitChunkPush::Request::get_stream_id() {
    return stream_id;
}
unsigned short nar::Messagetypes::WaitChunkPush::Request::get_randevous_port() {
    return rand_port;
}
long long int nar::Messagetypes::WaitChunkPush::Request::get_chunk_id() {
    return chunk_id;
}

long long int  nar::Messagetypes::WaitChunkPush::Request::get_chunk_size() {
    return chunk_size;
}
void nar::Messagetypes::WaitChunkPush::Request::send_mess(nar::Socket* skt) {
    nlohmann::json wpush_req_send;
    wpush_req_send["header"] = sendHead();
    wpush_req_send["payload"]["stream_id"] = this->stream_id;
    wpush_req_send["payload"]["rand_port"] = this->rand_port;
    wpush_req_send["payload"]["chunk_size"] =  this->chunk_size;
    wpush_req_send["payload"]["chunk_id"] = this->chunk_id;
    send_message(skt,wpush_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json wpush_req_recv = nlohmann::json::parse(temp);
    receive_message(wpush_req_recv);
    return;
}
void nar::Messagetypes::WaitChunkPush::Request::receive_message(nlohmann::json wpush_req_recv) {
    nlohmann::json head = wpush_req_recv["header"];
    recvFill(head);
    this->stream_id = wpush_req_recv["payload"]["stream_id"];
    this->rand_port = wpush_req_recv["payload"]["rand_port"];
    this->chunk_size = wpush_req_recv["payload"]["chunk_size"];
    this->chunk_id = wpush_req_recv["payload"]["chunk_id"];
    return;
}
nlohmann::json nar::Messagetypes::WaitChunkPush::Request::test_json() {
    nlohmann::json wpush_req_test;
    wpush_req_test["header"] = sendHead();
    wpush_req_test["payload"]["stream_id"] = this->stream_id;
    wpush_req_test["payload"]["rand_port"] = this->rand_port;
    wpush_req_test["payload"]["chunk_size"] =  this->chunk_size;
    wpush_req_test["payload"]["chunk_id"] = this->chunk_id;
    return wpush_req_test;
}

void nar::Messagetypes::WaitChunkPush::Response::send_mess(nar::Socket* skt) {
    nlohmann::json wpush_resp_send;
    wpush_resp_send["header"] = sendHead();
    send_message(skt,wpush_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json wpush_resp_recv = nlohmann::json::parse(temp);
    receive_message(wpush_resp_recv);
    return;
}

void nar::Messagetypes::WaitChunkPush::Response::receive_message(nlohmann::json wpush_resp_recv) {
    nlohmann::json head = wpush_resp_recv["header"];
    recvFill(head);
    return;
}
nlohmann::json nar::Messagetypes::WaitChunkPush::Response::test_json() {
    nlohmann::json wpush_resp_test;
    wpush_resp_test["header"] = sendHead();
    return wpush_resp_test;
}
