#include "IPCResponse.h"

long int nar::Messagetypes::IPCResponse::get_progress() {
    return progress;
}

std::string nar::Messagetypes::IPCResponse::get_process_name() {
    return process_name;
}

long int nar::Messagetypes::IPCResponse::get_status_code() {
    return status_code;
}

std::string nar::Messagetypes::IPCResponse::get_file_name() {
    return file_name;
}

void nar::Messagetypes::IPCResponse::set_progress(long int p) {
    progress = p;
}

void nar::Messagetypes::IPCResponse::set_process_name(std::string pn) {
    process_name = pn;
}

void nar::Messagetypes::IPCResponse::set_status_code(long int sc) {
    status_code = sc;
}

void nar::Messagetypes::IPCResponse::set_file_name(std::string fn) {
    file_name = fn;
}

nlohmann::json nar::Messagetypes::IPCResponse::get_myresponsejson() {
    nlohmann::json my_response_json;
    my_response_json["header"]["process_name"] = this -> process_name;
    my_response_json["payload"]["progress"] = this -> progress;
    my_response_json["payload"]["status_code"] = this -> status_code;
    my_response_json["payload"]["file_name"] = this -> file_name;
    return my_response_json;
}

void nar::Messagetypes::IPCResponse::send_message_progress(nar::Socket* skt, int p) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = this -> process_name;
    json_to_sent["payload"]["status_code"] = this -> status_code;
    json_to_sent["payload"]["progress"] = p;
    json_to_sent["payload"]["file_name"] = this -> file_name;
    send_message(skt,json_to_sent.dump());
    return;
}
