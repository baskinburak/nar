//
// Created by fatih on 09.05.2017.
//

#include "DaemonShutdown.h"


void nar::MessageTypes::DaemonShutdown::Request::send_mess(nar::Socket* skt) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    keep_req_send["payload"]["machine_id"] = this->_mac_id;
    send_message(skt,keep_req_send.dump());
    return;
}
void nar::MessageTypes::DaemonShutdown::Request::receive_message(std::string & message){
    try {
        nlohmann::json daemon_shutdown_recv = nlohmann::json::parse(message);
        nlohmann::json head = daemon_shutdown_recv["header"];
        recv_fill(head);
        this->_mac_id = daemon_shutdown_recv["payload"]["machine_id"];
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("daemon shutdown message has problems");
    }

    return;
}
std::string & nar::MessageTypes::DaemonShutdown::Request::get_machine_id() {
    return this->_mac_id;
}
nlohmann::json nar::MessageTypes::DaemonShutdown::Request::test_json() {
    nlohmann::json keep_req_test;
    return keep_req_test;
}
