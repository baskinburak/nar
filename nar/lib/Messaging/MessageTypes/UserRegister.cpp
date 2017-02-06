#include "UserRegister.h"
std::string& nar::messagetypes::UserRegister::Request::get_username(){
    return username;
}
std::string& nar::messagetypes::UserRegister::Request::get_aes(){
    return aes;
}
void nar::messagetypes::UserRegister::Request::sendMessage() {
    nlohmann::json usrreg_req_send;
    usrreg_req_send["header"] = sendHead();
    usrreg_req_send["payload"]["username"] = this->username;
    usrreg_req_send["payload"]["aes"] = this->aes;
    std::cout<<usrreg_req_send.dump()<<std::endl;
    return;

}
void nar::messagetypes::UserRegister::Request::receiveMessage(nlohmann::json usrreg_req_recv){
    nlohmann::json head = usrreg_req_recv["header"];
    recvFill(head);
    this->username = usrreg_req_recv["payload"]["username"];
    this->aes = usrreg_req_recv["payload"]["aes"];
    return;
}
void nar::messagetypes::UserRegister::Response::sendMessage() {
    nlohmann::json usrreg_resp_send;
    usrreg_resp_send["header"] = sendHead();
    std::cout<<usrreg_resp_send.dump()<<std::endl;
    return;
}

void nar::messagetypes::UserRegister::Response::receiveMessage(nlohmann::json usrreg_resp_recv){
    nlohmann::json head = usrreg_resp_recv["header"];
    recvFill(head);
    return;
}
