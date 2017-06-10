#include "IPCDeleteFile.h"

std::string nar::MessageTypes::IPCDeleteFile::Request::get_file_name(){
    return _file_name;
}

std::string nar::MessageTypes::IPCDeleteFile::Request::get_dest_dir() {
    return _dest_dir;
}
void nar::MessageTypes::IPCDeleteFile::Request::set_file_name(std::string dn){
    this->_file_name = dn;
}
void nar::MessageTypes::IPCDeleteFile::Request::set_dest_dir(std::string dd) {
    this->_dest_dir = dd;
}

nlohmann::json nar::MessageTypes::IPCDeleteFile::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "delete_file";
    json_to_sent["payload"]["file_name"] = _file_name;
    json_to_sent["payload"]["dir_name"] = _dest_dir;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCDeleteFile::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["file_name"] = this->_file_name;
    jsn["payload"]["dir_name"] = this->_dest_dir;
    return jsn;
}

void nar::MessageTypes::IPCDeleteFile::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_file_name = jsn["payload"]["file_name"];
    this->_dest_dir = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCDeleteFile::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCDeleteFile::Request::print_loop(nar::Socket* skt) {
    while (true) {

        std::string tmp = get_message(*skt);
       // std::cout << tmp << std::endl;
        nlohmann::json received = nlohmann::json::parse(tmp);
      //  std::cout << "PARSED" << std::endl;


        if( received["header"]["reply_to"] == std::string("END")  ) {
                break;
        }

        int stat = received["payload"]["status_code"];
        if (stat != 200) {
            if(stat/100 == 3) {
                std::cout<<"Your request was not complete or was wrong --- stat"<< stat<<std::endl;
                break;
            } else  if(stat/100 == 4) {
                std::cout<<"Problem With Server--- stat"<< stat<<std::endl;
                break;
            } else  if(stat/100 == 5) {
                std::cout<<"Some things went wrong in server --- stat"<< stat<<std::endl;
                break;
            } else if (stat == 300) {
                std::cout << "The file you specified does not exist in the nar system." << std::endl;
                break;
            } else  if(stat/100 == 6) {
                std::cout<<"Some things went wrong in daemon --- stat"<< stat<<std::endl;
                break;
            }
        } else std::cout << "SUCCESS" << std::endl;
    }

    return;
}

nlohmann::json nar::MessageTypes::IPCDeleteFile::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    return resp_json;
}
