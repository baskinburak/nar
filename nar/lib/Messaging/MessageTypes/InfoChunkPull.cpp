#include "InfoChunkPull.h"

long long int nar::MessageTypes::InfoChunkPull::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::MessageTypes::InfoChunkPull::Request::get_success() {
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
void nar::MessageTypes::InfoChunkPull::Request::receive_message(nlohmann::json ipull_req_recv) {
    nlohmann::json head = ipull_req_recv["header"];
    recv_fill(head);
    this->chunk_id = ipull_req_recv["payload"]["chunk_id"];
    this->success = ipull_req_recv["payload"]["success"];
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
    send_message(skt,ipull_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipull_resp_recv = nlohmann::json::parse(temp);
    receive_message(ipull_resp_recv);
    return;
}

void nar::MessageTypes::InfoChunkPull::Response::receive_message(nlohmann::json ipull_resp_recv) {
    nlohmann::json head = ipull_resp_recv["header"];
    recv_fill(head);
    return;
}

nlohmann::json nar::MessageTypes::InfoChunkPull::Response::test_json() {
    nlohmann::json ipull_resp_test;
    ipull_resp_test["header"] = send_head();
    return ipull_resp_test;
}
