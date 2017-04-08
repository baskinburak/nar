#include "MachineRegister.h"


void nar::MessageTypes::MachineRegister::Request::send_mess(nar::Socket* skt ,nar::MessageTypes::MachineRegister::Response & resp){
    nlohmann::json macreg_req_send;
    macreg_req_send["header"] = send_head();
    macreg_req_send["payload"]["machine_quota"] = this->_machine_quota;
    macreg_req_send["payload"]["machine_diskspace"] = this->_machine_diskspace;
    send_message(skt,macreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json macreg_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(macreg_req_recv);
    return;
}
void nar::MessageTypes::MachineRegister::Request::receive_message(std::string& msg){
    nlohmann::json  macreg_req_recv = nlohmann::json::parse(msg);
    nlohmann::json head = macreg_req_recv["header"];
    this->_machine_quota = macreg_req_recv["payload"]["machine_quota"];
    this->_machine_diskspace = macreg_req_recv["payload"]["machine_disk_space"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::MachineRegister::Request::test_json() {
    nlohmann::json macreg_req_test;
    return macreg_req_test;
}

std::string & nar::MessageTypes::MachineRegister::Response::get_machine_id(){
    return _machine_id;
}
void nar::MessageTypes::MachineRegister::Response::send_mess(nar::Socket* skt){
    nlohmann::json macreg_resp_send;
    macreg_resp_send["header"] = send_head();
    macreg_resp_send["payload"]["machine_id"] = this->_machine_id;
    send_message(skt,macreg_resp_send.dump());
    return;
}
void nar::MessageTypes::MachineRegister::Response::receive_message(nlohmann::json macreg_resp_recv){
    nlohmann::json head = macreg_resp_recv["header"];
    recv_fill(head);
    this->_machine_id = macreg_resp_recv["payload"]["machine_id"];
    return;
}
nlohmann::json nar::MessageTypes::MachineRegister::Response::test_json() {
    nlohmann::json macreg_resp_test;
    return macreg_resp_test;
}
long long int nar::MessageTypes::MachineRegister::Request::get_machine_quota() {
    return _machine_quota;
}
long long int nar::MessageTypes::MachineRegister::Request::get_machine_diskspace() {
    return _machine_diskspace;
}
