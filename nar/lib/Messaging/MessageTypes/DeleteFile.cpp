//
// Created by fatih on 18.04.2017.
//
#include "DeleteFile.h"

void nar::MessageTypes::DeleteFile::Request::send_mess(nar::Socket* skt, nar::MessageTypes::DeleteFile::Response & resp) {
    nlohmann::json mkdir_req_send;
    mkdir_req_send["header"] = send_head();
    mkdir_req_send["payload"]["file_name"] = this->_file_name;
    mkdir_req_send["payload"]["dest_dir"] = this->_dest_dir;
    send_message(skt,mkdir_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json mkdir_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(mkdir_req_recv);
    return;
}
void nar::MessageTypes::DeleteFile::Request::receive_message(std::string& msg){
    auto keep_req_recv = nlohmann::json::parse(msg);
    nlohmann::json head = keep_req_recv["header"];
    recv_fill(head);
    this->_file_name = keep_req_recv["payload"]["file_name"];
    this->_dest_dir = keep_req_recv["payload"]["dest_dir"];
    return;
}
nlohmann::json nar::MessageTypes::DeleteFile::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::DeleteFile::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::DeleteFile::Response::receive_message(nlohmann::json& keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recv_fill(head);

    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::ServerSocketAuthenticationError("Server can not authenticate socket created for this user", _status_code);
    }
    return;
}

nlohmann::json nar::MessageTypes::DeleteFile::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}

std::string& nar::MessageTypes::DeleteFile::Request::get_file_name() {
    return this->_file_name;
}
std::string& nar::MessageTypes::DeleteFile::Request::get_dest_dir() {
    return this->_dest_dir;
}
