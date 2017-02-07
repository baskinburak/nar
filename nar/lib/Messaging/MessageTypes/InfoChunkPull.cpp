#include "InfoChunkPull.h"

long long int nar::Messagetypes::InfoChunkPull::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::Messagetypes::InfoChunkPull::Request::get_success() {
    return success;
}

void nar::Messagetypes::InfoChunkPull::Request::send_mess(nar::Socket* skt) {
    nlohmann::json ipull_req_send;
    ipull_req_send["header"] = sendHead();
    ipull_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_send["payload"]["success"] = this->success;
    send_message(skt,ipull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipull_req_recv = nlohmann::json::parse(temp);
    receive_message(ipull_req_recv);
    return;

}
void nar::Messagetypes::InfoChunkPull::Request::receive_message(nlohmann::json ipull_req_recv) {
    nlohmann::json head = ipull_req_recv["header"];
    recvFill(head);
    this->chunk_id = ipull_req_recv["payload"]["chunk_id"];
    this->success = ipull_req_recv["payload"]["success"];
    return;
}

nlohmann::json nar::Messagetypes::InfoChunkPull::Request::test_json() {
    nlohmann::json ipull_req_test;
    ipull_req_test["header"] = sendHead();
    ipull_req_test["payload"]["chunk_id"] = this->chunk_id;
    ipull_req_test["payload"]["success"] = this->success;
    return ipull_req_test;
}

void nar::Messagetypes::InfoChunkPull::Response::send_mess(nar::Socket* skt) {
    nlohmann::json ipull_resp_send;
    ipull_resp_send["header"] = sendHead();
    send_message(skt,ipull_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipull_resp_recv = nlohmann::json::parse(temp);
    receive_message(ipull_resp_recv);
    return;
}

void nar::Messagetypes::InfoChunkPull::Response::receive_message(nlohmann::json ipull_resp_recv) {
    nlohmann::json head = ipull_resp_recv["header"];
    recvFill(head);
    return;
}

nlohmann::json nar::Messagetypes::InfoChunkPull::Response::test_json() {
    nlohmann::json ipull_resp_test;
    ipull_resp_test["header"] = sendHead();
    return ipull_resp_test;
}
