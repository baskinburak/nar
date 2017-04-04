#include "WaitChunkPush.h"
long long int nar::MessageTypes::WaitChunkPush::Request::get_stream_id() {
    return stream_id;
}
unsigned short nar::MessageTypes::WaitChunkPush::Request::get_randevous_port() {
    return rand_port;
}
long long int nar::MessageTypes::WaitChunkPush::Request::get_chunk_id() {
    return chunk_id;
}

long long int  nar::MessageTypes::WaitChunkPush::Request::get_chunk_size() {
    return chunk_size;
}
void nar::MessageTypes::WaitChunkPush::Request::send_mess(nar::Socket* skt, nar::MessageTypes::WaitChunkPush::Response & resp) {
    nlohmann::json wpush_req_send;
    wpush_req_send["header"] = send_head();
    wpush_req_send["payload"]["stream_id"] = this->stream_id;
    wpush_req_send["payload"]["rand_port"] = this->rand_port;
    wpush_req_send["payload"]["chunk_size"] =  this->chunk_size;
    wpush_req_send["payload"]["chunk_id"] = this->chunk_id;
    std::cout << "GELDIMMMM BENNNNNNNNN :OOOOOOO" << std::endl;
    send_message(skt,wpush_req_send.dump());
    std::string temp = get_message(skt);
    std::cout << "WAITCHUNK REQ GOT RESP" << std::endl;
    nlohmann::json wpush_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(wpush_req_recv);
    return;
}
void nar::MessageTypes::WaitChunkPush::Request::receive_message(nlohmann::json wpush_req_recv) {
    nlohmann::json head = wpush_req_recv["header"];
    recv_fill(head);
    this->stream_id = wpush_req_recv["payload"]["stream_id"];
    this->rand_port = wpush_req_recv["payload"]["rand_port"];
    this->chunk_size = wpush_req_recv["payload"]["chunk_size"];
    this->chunk_id = wpush_req_recv["payload"]["chunk_id"];
    return;
}
nlohmann::json nar::MessageTypes::WaitChunkPush::Request::test_json() {
    nlohmann::json wpush_req_test;
    wpush_req_test["header"] = send_head();
    wpush_req_test["payload"]["stream_id"] = this->stream_id;
    wpush_req_test["payload"]["rand_port"] = this->rand_port;
    wpush_req_test["payload"]["chunk_size"] =  this->chunk_size;
    wpush_req_test["payload"]["chunk_id"] = this->chunk_id;
    return wpush_req_test;
}

void nar::MessageTypes::WaitChunkPush::Response::send_mess(nar::Socket* skt) {
    nlohmann::json wpush_resp_send;
    wpush_resp_send["header"] = send_head();
    send_message(skt,wpush_resp_send.dump());
    return;
}

void nar::MessageTypes::WaitChunkPush::Response::receive_message(nlohmann::json wpush_resp_recv) {
    nlohmann::json head = wpush_resp_recv["header"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::WaitChunkPush::Response::test_json() {
    nlohmann::json wpush_resp_test;
    wpush_resp_test["header"] = send_head();
    return wpush_resp_test;
}
