#include "IPCBaseResponse.h"

long int nar::MessageTypes::IPCBaseResponse::get_progress() {
    return _progress;
}

std::string nar::MessageTypes::IPCBaseResponse::get_reply_to() {
    return _reply_to;
}

long int nar::MessageTypes::IPCBaseResponse::get_status_code() {
    return _status_code;
}

void nar::MessageTypes::IPCBaseResponse::set_progress(long int p) {
    _progress = p;
    return;
}

void nar::MessageTypes::IPCBaseResponse::set_reply_to(std::string pn) {
    _reply_to = pn;
    return;
}

void nar::MessageTypes::IPCBaseResponse::set_status_code(long int sc) {
    _status_code = sc;
    return;
}

nlohmann::json nar::MessageTypes::IPCBaseResponse::give_myresponsejson() {
    nlohmann::json my_response_json;
    try{
        my_response_json["header"]["reply_to"] = this -> _reply_to;
        my_response_json["payload"]["progress"] = this -> _progress;
        my_response_json["payload"]["status_code"] = this -> _status_code;
    } catch(...) {
        throw nar::Exception::MessageTypes::BadJSONRelatedProblemResponse("in IPCBaseResponse, we couldn't give the response json");
    }
    return my_response_json;
}

void nar::MessageTypes::IPCBaseResponse::send_message(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["reply_to"] = this -> _reply_to;
    json_to_sent["payload"]["status_code"] = this -> _status_code;
    json_to_sent["payload"]["progress"] = this -> _progress;
    nar::send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCBaseResponse::send_message_progress(nar::Socket* skt, int p) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["reply_to"] = this -> _reply_to;
    json_to_sent["payload"]["status_code"] = this -> _status_code;
    json_to_sent["payload"]["progress"] = p;
    nar::send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCBaseResponse::send_message_end(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    //std::cout << "sending from here" << std::endl;
    json_to_sent["header"]["reply_to"] = std::string("END");
    //std::cout << json_to_sent["header"]["reply_to"] << std::endl;
    nar::send_message(skt, json_to_sent.dump());
    return;
}
