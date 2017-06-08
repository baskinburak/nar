#include "KeepAlive.h"

void nar::MessageTypes::KeepAlive::Request::send_mess(nar::Socket* skt, nar::MessageTypes::KeepAlive::Response & resp) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    keep_req_send["payload"]["machine_id"] = this->_machine_id;
    send_message(skt, keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(keep_req_recv);
    return;
}
void nar::MessageTypes::KeepAlive::Request::receive_message(std::string& msg){
    auto keep_req_recv = nlohmann::json::parse(msg);
    try {
        nlohmann::json head = keep_req_recv["header"];
        recv_fill(head);
        this->_machine_id = keep_req_recv["payload"]["machine_id"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("KeepAlive Request message is not properly constructed.");
    }
    return;
}

nlohmann::json nar::MessageTypes::KeepAlive::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::KeepAlive::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    keep_resp_send["payload"]["size"] = _delete_list.size();
    for(int i=0; i<_delete_list.size(); i++) {
        keep_resp_send["payload"]["delete_list"][i] = _delete_list[i];
    }
    send_message(skt, keep_resp_send.dump());
    return;
}
void nar::MessageTypes::KeepAlive::Response::receive_message(nlohmann::json& keep_resp_recv) {
    try {
        nlohmann::json head = keep_resp_recv["header"];
        recv_fill(head);
        if(_status_code == 200) {
            unsigned long size = keep_resp_recv["payload"]["size"];
            for(int i= 0;i<size;i++) {
                _delete_list.push_back(keep_resp_recv["payload"]["delete_list"][i]);
            }
        }
    } catch(...) {        
        throw nar::Exception::MessageTypes::BadMessageReceive("KeepAlive Response message is not properly constructed.");
    }

    if(_status_code != 200) {
        throw nar::Exception::MessageTypes::ServerSocketAuthenticationError("Machine KeepAlive failed." , _status_code);
    }
    return;
}

nlohmann::json nar::MessageTypes::KeepAlive::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}

void nar::MessageTypes::KeepAlive::Response::set_delete_list(std::vector<std::string>& delete_list){
    this->_delete_list = delete_list;
    return;
}
std::vector<std::string>& nar::MessageTypes::KeepAlive::Response::get_delete_list() {
    return this->_delete_list;
}
void nar::MessageTypes::KeepAlive::Response::add_delete_list_element(std::string& ele) {
    this->_delete_list.push_back(ele);
    return;
}

std::string& nar::MessageTypes::KeepAlive::Request::get_machine_id() {
    return this->_machine_id;
}
