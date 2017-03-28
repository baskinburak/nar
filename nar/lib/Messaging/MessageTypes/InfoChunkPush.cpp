#include "InfoChunkPush.h"

long long int nar::MessageTypes::InfoChunkPush::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::MessageTypes::InfoChunkPush::Request::get_success() {
    return success;
}

void nar::MessageTypes::InfoChunkPush::Request::send_mess(nar::Socket* skt) {
    nlohmann::json ipush_req_send;
    ipush_req_send["header"] = send_head();
    ipush_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipush_req_send["payload"]["success"] = this->success;
    send_message(skt,ipush_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipush_req_recv = nlohmann::json::parse(temp);
    receive_message(ipush_req_recv);
    return;

}
void nar::MessageTypes::InfoChunkPush::Request::receive_message(nlohmann::json ipush_req_recv) {
    nlohmann::json head = ipush_req_recv["header"];
    recv_fill(head);
    this->chunk_id = ipush_req_recv["payload"]["chunk_id"];
    this->success = ipush_req_recv["payload"]["success"];
    return;
}

nlohmann::json nar::MessageTypes::InfoChunkPush::Request::test_json() {
    nlohmann::json ipush_req_test;
    ipush_req_test["header"] = send_head();
    ipush_req_test["payload"]["chunk_id"] = this->chunk_id;
    ipush_req_test["payload"]["success"] = this->success;
    return ipush_req_test;
}

void nar::MessageTypes::InfoChunkPush::Response::send_mess(nar::Socket* skt) {
    nlohmann::json ipush_resp_send;
    ipush_resp_send["header"] = send_head();
    send_message(skt,ipush_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipush_resp_recv = nlohmann::json::parse(temp);
    receive_message(ipush_resp_recv);
    return;
}

void nar::MessageTypes::InfoChunkPush::Response::receive_message(nlohmann::json ipush_resp_recv) {
    nlohmann::json head = ipush_resp_recv["header"];
    recv_fill(head);
    return;
}

nlohmann::json nar::MessageTypes::InfoChunkPush::Response::test_json() {
    nlohmann::json ipush_resp_test;
    ipush_resp_test["header"] = send_head();
    return ipush_resp_test;
}
