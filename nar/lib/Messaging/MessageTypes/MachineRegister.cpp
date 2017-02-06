#include "MachineRegister.h"


void nar::messagetypes::MachineRegister::Request::sendMessage(){
    nlohmann::json macreg_req_send;
    macreg_req_send["header"] = sendHead();
    std::cout<<macreg_req_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::MachineRegister::Request::receiveMessage(nlohmann::json macreg_req_recv){
    nlohmann::json head = macreg_req_recv["header"];
    recvFill(head);
    return;
}

void nar::messagetypes::MachineRegister::Response::sendMessage(){
    nlohmann::json macreg_resp_send;
    macreg_resp_send["header"] = sendHead();
    macreg_resp_send["payload"]["machine_id"] = this->machine_id;
    std::cout<<macreg_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::MachineRegister::Response::receiveMessage(nlohmann::json macreg_resp_recv){
    nlohmann::json head = macreg_resp_recv["header"];
    recvFill(head);
    this->machine_id = macreg_resp_recv["payload"]["machine_id"];
    return;
}



std::string & nar::messagetypes::MachineRegister::Response::getMachineId(){
    return machine_id;
}
