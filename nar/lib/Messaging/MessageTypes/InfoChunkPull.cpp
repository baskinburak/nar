#include "InfoChunkPull.h"

long long int nar::MessageTypes::InfoChunkPull::Request::get_chunk_id() {
    return chunk_id;
}

int nar::MessageTypes::InfoChunkPull::Request::get_success() {
    return success;
}

void nar::MessageTypes::InfoChunkPull::Request::send_mess(nar::Socket* skt) {
    nlohmann::json ipull_req_send;
    ipull_req_send["header"] = send_head();
    ipull_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_send["payload"]["success"] = this->success;
    send_message(skt,ipull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipull_req_recv = nlohmann::json::parse(temp);
    receive_message(ipull_req_recv);
    return;

}

void nar::MessageTypes::InfoChunkPull::Request::send_mess(nar::Socket* skt, nar::MessageTypes::InfoChunkPull::Response & resp) {
    nlohmann::json ipull_req_send;
    ipull_req_send["header"] = send_head();
    ipull_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_send["payload"]["success"] = this->success;
    send_message(skt,ipull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipull_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(ipull_req_recv);
    return;
}


void nar::MessageTypes::InfoChunkPull::Request::receive_message(std::string& _ipull_req_recv) {
    try {
        ipull_req_recv = nlohmann::parse(_ipull_req_recv);
        nlohmann::json head = ipull_req_recv["header"];
        recv_fill(head);
        this->chunk_id = ipull_req_recv["payload"]["chunk_id"];
        this->success = ipull_req_recv["payload"]["success"];
    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("InfoChunkPull Request is constructed badly");
    }
    return;
}

nlohmann::json nar::MessageTypes::InfoChunkPull::Request::test_json() {
    nlohmann::json ipull_req_test;
    ipull_req_test["header"] = send_head();
    ipull_req_test["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_test["payload"]["success"] = this->success;
    return ipull_req_test;
}

void nar::MessageTypes::InfoChunkPull::Response::send_mess(nar::Socket* skt) {
    nlohmann::json ipull_resp_send;
    ipull_resp_send["header"] = send_head();
    ipull_resp_send["payload"]["stream_id"] = this->_stream_id;
    send_message(skt,ipull_resp_send.dump());
    return;
}

void nar::MessageTypes::InfoChunkPull::Response::receive_message(nlohmann::json ipull_resp_recv) {
    try{
        nlohmann::json head = ipull_resp_recv["header"];
        recv_fill(head);
        this->_stream_id = ipull_resp_recv["payload"]["stream_id"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("info chunk pull receive failure");
    }


    return;
}

long long int nar::MessageTypes::InfoChunkPull::Response::get_stream_id() {
    return this->_stream_id;
}

nlohmann::json nar::MessageTypes::InfoChunkPull::Response::test_json() {
    nlohmann::json ipull_resp_test;
    ipull_resp_test["header"] = send_head();
    return ipull_resp_test;
}
