#include "DirInfo.h"

std::string& nar::MessageTypes::DirInfo::Request::get_dir() {
    return dir;
}

std::vector<struct nar::MessageTypes::DirInfo::Response::DirListElement>& nar::MessageTypes::DirInfo::Response::get_elements() {
    return elements;
}

void nar::MessageTypes::DirInfo::Response::add_element(struct nar::MessageTypes::DirInfo::Response::DirListElement& ele) {
    elements.push_back(ele);
}

void nar::MessageTypes::DirInfo::Response::add_element(std::string ct, long long int ei, std::string en, unsigned long long int s, bool t) {
    struct DirListElement ele = {
        ct,
        ei,
        en,
        s,
        t
    };
    elements.push_back(ele);
}
void nar::MessageTypes::DirInfo::Request::send_mess(nar::Socket* skt ,nar::MessageTypes::DirInfo::Response & resp){
    nlohmann::json dir_req_send;
    dir_req_send["header"] = send_head();
    dir_req_send["payload"]["dir_name"] = dir;
    send_message(skt,dir_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json dir_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(dir_req_recv);
    return;
}
void nar::MessageTypes::DirInfo::Request::receive_message(std::string msg){
    nlohmann::json dir_req_recv = nlohmann::json::parse(msg);

    try {
        nlohmann::json head = dir_req_recv["header"];
        recv_fill(head);
        dir = dir_req_recv["payload"]["dir_name"];
    } catch(nar::Exception::MessageTypes::RequestRecvFillError exp) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Dir info request is badly constructed");
    }
    if(dir.empty()) {
        throw nar::Exception::MessageTypes::BadMessageReceive("dir cant be empty");
    }
    return;
}

nlohmann::json nar::MessageTypes::DirInfo::Request::test_json() {
    nlohmann::json dir_req_test;
    dir_req_test["header"] = send_head();
    dir_req_test["payload"]["dir_name"] = dir;
    return dir_req_test;
}
void nar::MessageTypes::DirInfo::Response::send_mess(nar::Socket* skt){

    nlohmann::json dir_resp_send;
    dir_resp_send["header"] = send_head();
    dir_resp_send["payload"]["item_list"] = nlohmann::json::array();
    dir_resp_send["payload"]["size"] = elements.size();
    for(int i = 0;i<elements.size();i++){
        dir_resp_send["payload"]["item_list"][i]["change_time"] = elements[i].change_time;
        dir_resp_send["payload"]["item_list"][i]["entity_id"] = elements[i].entity_id;
        dir_resp_send["payload"]["item_list"][i]["entity_name"] = elements[i].entity_name;
        dir_resp_send["payload"]["item_list"][i]["entity_size"] = elements[i].entity_size;
        dir_resp_send["payload"]["item_list"][i]["type"] = elements[i].type;
    }
    send_message(skt,dir_resp_send.dump());
    return;
}
nlohmann::json nar::MessageTypes::DirInfo::Response::test_json(){

    nlohmann::json dir_resp_test;
    dir_resp_test["header"] = send_head();
    dir_resp_test["payload"]["item_list"] = nlohmann::json::array();
    dir_resp_test["payload"]["size"] = elements.size();
    for(int i = 0;i<elements.size();i++){
        dir_resp_test["payload"]["item_list"][i]["change_time"] = elements[i].change_time;
        dir_resp_test["payload"]["item_list"][i]["entity_id"] = elements[i].entity_id;
        dir_resp_test["payload"]["item_list"][i]["entity_name"] = elements[i].entity_name;
        dir_resp_test["payload"]["item_list"][i]["entity_size"] = elements[i].entity_size;
        dir_resp_test["payload"]["item_list"][i]["type"] = elements[i].type;
    }
    return dir_resp_test;
}

void nar::MessageTypes::DirInfo::Response::receive_message(nlohmann::json dir_resp_recv){

    try {
        nlohmann::json head = dir_resp_recv["header"];
        recv_fill(head);
    }
    catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("problematic header for dir info response receive");

    }
    int stat = get_status_code();
    if(stat/100 == 3) {
        throw nar::Exception::MessageTypes::BadRequest("Your request was not complete or was wrong", stat);
    } else  if(stat/100 == 4) {
        throw nar::Exception::MessageTypes::InternalServerDatabaseError("Database problem", stat);
    } else  if(stat/100 == 5) {
        throw nar::Exception::MessageTypes::InternalServerError("Some things went wrong in server", stat);
    }
    try{
        int size = dir_resp_recv["payload"]["size"];
        for(int i=0;i<size;i++){
            std::string tchange_time = dir_resp_recv["payload"]["item_list"][i]["change_time"];
            long long int tentity_id = dir_resp_recv["payload"]["item_list"][i]["entity_id"];
            std::string tentity_name = dir_resp_recv["payload"]["item_list"][i]["entity_name"];
            unsigned long long int tentity_size =dir_resp_recv["payload"]["item_list"][i]["entity_size"];
            bool ttype = dir_resp_recv["payload"]["item_list"][i]["type"];
            this->add_element(tchange_time, tentity_id, tentity_name, tentity_size, ttype);
        }
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("DirInfo message receive has problems");
    }

    return;
}
