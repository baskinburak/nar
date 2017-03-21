#include "UserRegister.h"
std::string& nar::MessageTypes::UserRegister::Request::get_username(){
    return username;
}
std::string& nar::MessageTypes::UserRegister::Request::get_aes(){
    return aes;
}
void nar::MessageTypes::UserRegister::Request::send_mess(nar::Socket* skt, nar::MessageTypes::UserRegister::Response & resp) {
    nlohmann::json usrreg_req_send;
    usrreg_req_send["header"] = send_head();
    usrreg_req_send["payload"]["username"] = this->username;
    usrreg_req_send["payload"]["aes"] = this->aes;
    send_message(skt,usrreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json usrreg_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(usrreg_req_recv);
    return;

}
nlohmann::json nar::MessageTypes::UserRegister::Request::test_json() {
    nlohmann::json usrreg_req_test;
    usrreg_req_test["header"] = send_head();
    usrreg_req_test["payload"]["username"] = this->username;
    usrreg_req_test["payload"]["aes"] = this->aes;
    return usrreg_req_test;
}


void nar::MessageTypes::UserRegister::Request::receive_message(nlohmann::json usrreg_req_recv){
    nlohmann::json head = usrreg_req_recv["header"];
    recv_fill(head);
    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::UserNameAlreadyExist("User name already exists", _status_code);
    }
    this->username = usrreg_req_recv["payload"]["username"];
    this->aes = usrreg_req_recv["payload"]["aes"];
    return;
}
void nar::MessageTypes::UserRegister::Response::send_mess(nar::Socket* skt) {
    nlohmann::json usrreg_resp_send;
    usrreg_resp_send["header"] = send_head();
    send_message(skt,usrreg_resp_send.dump());
    return;
}

void nar::MessageTypes::UserRegister::Response::receive_message(nlohmann::json usrreg_resp_recv){
    nlohmann::json head = usrreg_resp_recv["header"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::UserRegister::Response::test_json() {
    nlohmann::json usrreg_resp_test;
    usrreg_resp_test["header"] = send_head();
    return usrreg_resp_test;
}
