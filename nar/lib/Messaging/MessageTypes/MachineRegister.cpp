#include "MachineRegister.h"


void nar::Messagetypes::MachineRegister::Request::send_mess(nar::Socket* skt){
    nlohmann::json macreg_req_send;
    macreg_req_send["header"] = sendHead();
    send_message(skt,macreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json macreg_req_recv = nlohmann::json::parse(temp);
    receive_message(macreg_req_recv);
    return;
}
void nar::Messagetypes::MachineRegister::Request::receive_message(nlohmann::json macreg_req_recv){
    nlohmann::json head = macreg_req_recv["header"];
    recvFill(head);
    return;
}
nlohmann::json nar::Messagetypes::MachineRegister::Request::test_json() {
    nlohmann::json macreg_req_test;
    macreg_req_test["header"] = sendHead();
    return macreg_req_test;
}

std::string & nar::Messagetypes::MachineRegister::Response::get_machine_id(){
    return machine_id;
}
void nar::Messagetypes::MachineRegister::Response::send_mess(nar::Socket* skt){
    nlohmann::json macreg_resp_send;
    macreg_resp_send["header"] = sendHead();
    macreg_resp_send["payload"]["machine_id"] = this->machine_id;
    send_message(skt,macreg_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json macreg_resp_recv = nlohmann::json::parse(temp);
    receive_message(macreg_resp_recv);
    return;
}
void nar::Messagetypes::MachineRegister::Response::receive_message(nlohmann::json macreg_resp_recv){
    nlohmann::json head = macreg_resp_recv["header"];
    recvFill(head);
    this->machine_id = macreg_resp_recv["payload"]["machine_id"];
    return;
}
nlohmann::json nar::Messagetypes::MachineRegister::Response::test_json() {
    nlohmann::json macreg_resp_test;
    macreg_resp_test["header"] = sendHead();
    macreg_resp_test["payload"]["machine_id"] = this->machine_id;
    return macreg_resp_test;
}
