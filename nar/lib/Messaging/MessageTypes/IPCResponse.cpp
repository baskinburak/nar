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

nlohmann::json nar::Messagetypes::IPCResponse::loop_send_message(nar::Socket* skt) {
    while(1){
        if(progress == 100){
            nlohmann::json json_to_sent;
            json_to_sent["header"]["process_name"] = "END";
            send_message(skt,json_to_sent.dump());
        }
        else{
            nlohmann::json json_to_sent;
            json_to_sent["header"]["process_name"] = process_name;
            json_to_sent["payload"]["status_code"] = status_code;
            json_to_sent["payload"]["progress"] = progress;
            json_to_sent["payload"]["file_name"] = file_name;
            send_message(skt,json_to_sent.dump());
        }
    }
}
