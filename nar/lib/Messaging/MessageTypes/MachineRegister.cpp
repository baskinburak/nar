#include "MachineRegister.h"


void nar::MessageTypes::MachineRegister::Request::send_mess(nar::Socket* skt){
    nlohmann::json macreg_req_send;
    macreg_req_send["header"] = send_head();
    send_message(skt,macreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json macreg_req_recv = nlohmann::json::parse(temp);
    receive_message(macreg_req_recv);
    return;
}
void nar::MessageTypes::MachineRegister::Request::receive_message(nlohmann::json macreg_req_recv){
    nlohmann::json head = macreg_req_recv["header"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::MachineRegister::Request::test_json() {
    nlohmann::json macreg_req_test;
    macreg_req_test["header"] = send_head();
    return macreg_req_test;
}

std::string & nar::MessageTypes::MachineRegister::Response::get_machine_id(){
    return machine_id;
}
void nar::MessageTypes::MachineRegister::Response::send_mess(nar::Socket* skt){
    nlohmann::json macreg_resp_send;
    macreg_resp_send["header"] = send_head();
    macreg_resp_send["payload"]["machine_id"] = this->machine_id;
    send_message(skt,macreg_resp_send.dump());
    return;
}
void nar::MessageTypes::MachineRegister::Response::receive_message(nlohmann::json macreg_resp_recv){
    nlohmann::json head = macreg_resp_recv["header"];
    recv_fill(head);
    this->machine_id = macreg_resp_recv["payload"]["machine_id"];
    return;
}
nlohmann::json nar::MessageTypes::MachineRegister::Response::test_json() {
    nlohmann::json macreg_resp_test;
    macreg_resp_test["header"] = send_head();
    macreg_resp_test["payload"]["machine_id"] = this->machine_id;
    return macreg_resp_test;
}
