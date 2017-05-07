#include "DeleteMachineChunk.h"


void nar::MessageTypes::DeleteMachineChunk::Request::send_mess(nar::Socket* skt, nar::MessageTypes::DeleteMachineChunk::Response & resp) {
    nlohmann::json del_mac_chu_send;
    del_mac_chu_send["header"] = send_head();
    del_mac_chu_send["payload"]["chunk_id"] = this->_chunk_id;
    send_message(skt,del_mac_chu_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json del_mac_chu_recv = nlohmann::json::parse(temp);
    resp.receive_message(del_mac_chu_recv);
    return;
}
void nar::MessageTypes::DeleteMachineChunk::Request::receive_message(std::string& msg){
    try {
        auto keep_req_recv = nlohmann::json::parse(msg);
        nlohmann::json head = keep_req_recv["header"];
        recv_fill(head);
        this->_chunk_id = keep_req_recv["payload"]["chunk_id"];
    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("delete machine chunk request message receive is problematic");
    }
    if(this->_chunk_id.empty()) {
        throw nar::Exception::MessageTypes::BadMessageReceive("chunk_id can not be empty");
    }


    return;
}
nlohmann::json nar::MessageTypes::DeleteMachineChunk::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::DeleteMachineChunk::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::DeleteMachineChunk::Response::receive_message(nlohmann::json& keep_resp_recv){
    try {
        nlohmann::json head = keep_resp_recv["header"];
        recv_fill(head);

    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("delete machine chunk response message receive is problematic");
    }


    int stat = get_status_code();
    if(stat/100 == 6) {
        throw nar::Exception::MessageTypes::InternalDaemonError("Daemon could not delete the chunk",stat);
    }
    return;
}

nlohmann::json nar::MessageTypes::DeleteMachineChunk::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}

std::string& nar::MessageTypes::DeleteMachineChunk::Request::get_chunk_id() {
    return this->_chunk_id;
}
