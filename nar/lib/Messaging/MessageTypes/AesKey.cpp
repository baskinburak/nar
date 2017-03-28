#include "AesKey.h"

void nar::MessageTypes::AesKey::Request::send_mess(nar::Socket* skt, nar::MessageTypes::AesKey::Response & resp) {
    nlohmann::json keep_req_send;
    keep_req_send["header"] = send_head();
    send_message(skt,keep_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json keep_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(keep_req_recv);
    return;
}
void nar::MessageTypes::AesKey::Request::receive_message(nlohmann::json keep_req_recv){
    nlohmann::json head = keep_req_recv["header"];
    recv_fill(head);
    return;
}
nlohmann::json nar::MessageTypes::AesKey::Request::test_json() {
    nlohmann::json keep_req_test;
    keep_req_test["header"] = send_head();
    return keep_req_test;
}

void nar::MessageTypes::AesKey::Response::send_mess(nar::Socket* skt) {
    nlohmann::json keep_resp_send;
    keep_resp_send["header"] = send_head();
    keep_resp_send["payload"]["aes"] = _aes;
    send_message(skt,keep_resp_send.dump());
    return;
}
void nar::MessageTypes::AesKey::Response::receive_message(nlohmann::json keep_resp_recv){
    nlohmann::json head = keep_resp_recv["header"];
    recv_fill(head);
    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::ServerSocketAuthenticationError("Server can not authenticate socket created for this user", _status_code);
    } else if(_status_code == 301) {
        throw nar::Exception::MessageTypes::UserDoesNotExist("This user does not exist for nar system so you can not get aes key for it", _status_code);
    }
    _aes = keep_resp_recv["payload"]["aes"];
    return;
}

nlohmann::json nar::MessageTypes::AesKey::Response::test_json() {
    nlohmann::json keep_resp_test;
    keep_resp_test["header"] = send_head();
    return keep_resp_test;
}

std::string & nar::MessageTypes::AesKey::Response::get_aes_key() {
    return _aes;
}
