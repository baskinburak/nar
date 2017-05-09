#include "IPCStatus.h"

std::string nar::MessageTypes::IPCStatus::Response::get_nar_folder() {
    return _nar_folder;
}

std::string nar::MessageTypes::IPCStatus::Response::get_file_folder() {
    return _file_folder;
}

std::string nar::MessageTypes::IPCStatus::Response::get_server_ip() {
    return _server_ip;
}

std::string nar::MessageTypes::IPCStatus::Response::get_server_port() {
    return _server_port;
}

std::string nar::MessageTypes::IPCStatus::Response::get_machine_id() {
    return _machine_id;
}

nlohmann::json nar::MessageTypes::IPCStatus::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "status";
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCStatus::Request::generate_json() {
	nlohmann::json jsn = IPCBaseRequest::generate_json();
    return jsn;
}
void nar::MessageTypes::IPCStatus::Request::populate_object(std::string& jsn_str) {
	auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
}

void nar::MessageTypes::IPCStatus::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCStatus::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();

    resp_json["payload"]["nar_folder"] = get_nar_folder();
    resp_json["payload"]["file_folder"] = get_file_folder();
    resp_json["payload"]["server_ip"] = get_server_ip();
    resp_json["payload"]["server_port"] = get_server_port();
    resp_json["payload"]["machine_id"] = get_machine_id();

    return resp_json;
}

void nar::MessageTypes::IPCStatus::Response::send_message(nar::Socket* skt) {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["reply_to"] = get_reply_to();
    json_to_sent["payload"]["progress"] = get_progress();
    json_to_sent["payload"]["status_code"] = get_status_code();

    json_to_sent["payload"]["nar_folder"] = get_nar_folder();
    json_to_sent["payload"]["file_folder"] = get_file_folder();
    json_to_sent["payload"]["server_ip"] = get_server_ip();
    json_to_sent["payload"]["server_port"] = get_server_port();
    json_to_sent["payload"]["machine_id"] = get_machine_id();
    nar::send_message(skt, json_to_sent.dump());
    return;
}



void nar::MessageTypes::IPCStatus::Request::print_loop(nar::Socket* skt) {
    bool isError = true;
    while(true){
        std::string tmp;
        nlohmann::json received;
        try {
            tmp = get_message(*skt);
        } catch(nar::Exception::Socket::SystemError& Exp) {
            throw;
        } catch(...) {
            throw nar::Exception::LowLevelMessaging::Error("Low level messaging error in print_loop.");
        }

        try {
            received = nlohmann::json::parse(tmp);
        } catch(...) {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message not JSON received in print_loop");
        }
        int statcode;
        try {
            if(received["header"]["reply_to"] == std::string("END"))
                break;
            statcode = received["payload"]["status_code"];
        }
        catch ( ... ) {
            throw nar::Exception::MessageTypes::BadMessageReceive("Bad message received in print_loop");
        }


        if(statcode == 200) {
            std::cout << "------------all your info------------" << std::endl;
            std::cout << received["payload"]["nar_folder"] << std::endl;
            std::cout << received["payload"]["file_folder"] << std::endl;
            std::cout << received["payload"]["server_ip"] << std::endl;
            std::cout << received["payload"]["server_port"] << std::endl;
            std::cout << received["payload"]["machine_id"] << std::endl;


        } else if(statcode == 601) {
            std::cout << "Cannot connect to the server." << std::endl;
        } else {
            std::cout << "Unprinted error." << std::endl;
        }
    }
}
