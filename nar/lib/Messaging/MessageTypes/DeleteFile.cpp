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
    std::cout << "temp" << temp << std::endl;
    nlohmann::json mkdir_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(mkdir_req_recv);
    return;
}
void nar::MessageTypes::DeleteFile::Request::receive_message(std::string& msg){
    try {
        auto keep_req_recv = nlohmann::json::parse(msg);
        nlohmann::json head = keep_req_recv["header"];
        recv_fill(head);
        this->_file_name = keep_req_recv["payload"]["file_name"];
        this->_dest_dir = keep_req_recv["payload"]["dest_dir"];
    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Delete file request bad receive");
    }

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
    try {
        nlohmann::json head = keep_resp_recv["header"];
        recv_fill(head);
    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Delete file response bad receive");
    }

    int stat = get_status_code();
    if(stat/100 == 3) {
        throw nar::Exception::MessageTypes::BadRequest("Your request was not complete or was wrong", stat);
    } else  if(stat/100 == 4) {
        throw nar::Exception::MessageTypes::InternalServerDatabaseError("Database problem", stat);
    } else  if(stat/100 == 5) {
        throw nar::Exception::MessageTypes::InternalServerError("Some things went wrong in server", stat);
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
