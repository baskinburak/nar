#include "UserRegister.h"
std::string& nar::Messagetypes::UserRegister::Request::get_username(){
    return username;
}
std::string& nar::Messagetypes::UserRegister::Request::get_aes(){
    return aes;
}
void nar::Messagetypes::UserRegister::Request::send_mess(nar::Socket* skt) {
    nlohmann::json usrreg_req_send;
    usrreg_req_send["header"] = sendHead();
    usrreg_req_send["payload"]["username"] = this->username;
    usrreg_req_send["payload"]["aes"] = this->aes;
    send_message(skt,usrreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json usrreg_req_recv = nlohmann::json::parse(temp);
    receive_message(usrreg_req_recv);
    return;

}
nlohmann::json nar::Messagetypes::UserRegister::Request::test_json() {
    nlohmann::json usrreg_req_test;
    usrreg_req_test["header"] = sendHead();
    usrreg_req_test["payload"]["username"] = this->username;
    usrreg_req_test["payload"]["aes"] = this->aes;
    return usrreg_req_test;
}


void nar::Messagetypes::UserRegister::Request::receive_message(nlohmann::json usrreg_req_recv){
    nlohmann::json head = usrreg_req_recv["header"];
    recvFill(head);
    this->username = usrreg_req_recv["payload"]["username"];
    this->aes = usrreg_req_recv["payload"]["aes"];
    return;
}
void nar::Messagetypes::UserRegister::Response::send_mess(nar::Socket* skt) {
    nlohmann::json usrreg_resp_send;
    usrreg_resp_send["header"] = sendHead();
    send_message(skt,usrreg_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json usrreg_resp_recv = nlohmann::json::parse(temp);
    receive_message(usrreg_resp_recv);
    return;
}

void nar::Messagetypes::UserRegister::Response::receive_message(nlohmann::json usrreg_resp_recv){
    nlohmann::json head = usrreg_resp_recv["header"];
    recvFill(head);
    return;
}
nlohmann::json nar::Messagetypes::UserRegister::Response::test_json() {
    nlohmann::json usrreg_resp_test;
    usrreg_resp_test["header"] = sendHead();
    return usrreg_resp_test;
}
