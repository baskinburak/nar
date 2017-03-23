#include "IPCBaseResponse.h"

long int nar::Messagetypes::IPCBaseResponse::get_progress() {
    return progress;
}

std::string nar::Messagetypes::IPCBaseResponse::get_process_name() {
    return process_name;
}

long int nar::Messagetypes::IPCBaseResponse::get_status_code() {
    return status_code;
}

void nar::Messagetypes::IPCBaseResponse::set_progress(long int p) {
    progress = p;
    return;
}

void nar::Messagetypes::IPCBaseResponse::set_process_name(std::string pn) {
    process_name = pn;
    return;
}

void nar::Messagetypes::IPCBaseResponse::set_status_code(long int sc) {
    status_code = sc;
    return;
}

nlohmann::json nar::Messagetypes::IPCBaseResponse::give_myresponsejson() {
    nlohmann::json my_response_json;
    my_response_json["header"]["process_name"] = this -> process_name;
    my_response_json["payload"]["progress"] = this -> progress;
    my_response_json["payload"]["status_code"] = this -> status_code;
    return my_response_json;
}

void nar::Messagetypes::IPCBaseResponse::send_message_progress(nar::Socket* skt, int p) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = this -> process_name;
    json_to_sent["payload"]["status_code"] = this -> status_code;
    json_to_sent["payload"]["progress"] = p;
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::Messagetypes::IPCBaseResponse::send_message_end(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["process_name"] = "END";
    send_message(skt, json_to_sent.dump());
    return;
}
