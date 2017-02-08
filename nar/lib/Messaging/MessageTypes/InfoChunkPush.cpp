#include "InfoChunkPush.h"

long long int nar::Messagetypes::InfoChunkPush::Request::get_chunk_id() {
    return chunk_id;
}

bool nar::Messagetypes::InfoChunkPush::Request::get_success() {
    return success;
}

void nar::Messagetypes::InfoChunkPush::Request::send_mess(nar::Socket* skt) {
    nlohmann::json ipush_req_send;
    ipush_req_send["header"] = sendHead();
    ipush_req_send["payload"]["chunk_id"] = this->chunk_id;
    ipush_req_send["payload"]["success"] = this->success;
    send_message(skt,ipush_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipush_req_recv = nlohmann::json::parse(temp);
    receive_message(ipush_req_recv);
    return;

}
void nar::Messagetypes::InfoChunkPush::Request::receive_message(nlohmann::json ipush_req_recv) {
    nlohmann::json head = ipush_req_recv["header"];
    recvFill(head);
    this->chunk_id = ipush_req_recv["payload"]["chunk_id"];
    this->success = ipush_req_recv["payload"]["success"];
    return;
}

nlohmann::json nar::Messagetypes::InfoChunkPush::Request::test_json() {
    nlohmann::json ipush_req_test;
    ipush_req_test["header"] = sendHead();
    ipush_req_test["payload"]["chunk_id"] = this->chunk_id;
    ipush_req_test["payload"]["success"] = this->success;
    return ipush_req_test;
}

void nar::Messagetypes::InfoChunkPush::Response::send_mess(nar::Socket* skt) {
    nlohmann::json ipush_resp_send;
    ipush_resp_send["header"] = sendHead();
    send_message(skt,ipush_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json ipush_resp_recv = nlohmann::json::parse(temp);
    receive_message(ipush_resp_recv);
    return;
}

void nar::Messagetypes::InfoChunkPush::Response::receive_message(nlohmann::json ipush_resp_recv) {
    nlohmann::json head = ipush_resp_recv["header"];
    recvFill(head);
    return;
}

nlohmann::json nar::Messagetypes::InfoChunkPush::Response::test_json() {
    nlohmann::json ipush_resp_test;
    ipush_resp_test["header"] = sendHead();
    return ipush_resp_test;
}
