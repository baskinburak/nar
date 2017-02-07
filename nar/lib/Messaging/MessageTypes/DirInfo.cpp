#include "DirInfo.h"

std::string& nar::Messagetypes::DirInfo::Request::get_dir() {
    return dir;
}

std::vector<struct nar::Messagetypes::DirInfo::Response::DirListElement>& nar::Messagetypes::DirInfo::Response::get_elements() {
    return elements;
}

void nar::Messagetypes::DirInfo::Response::add_element(struct nar::Messagetypes::DirInfo::Response::DirListElement& ele) {
    elements.push_back(ele);
}

void nar::Messagetypes::DirInfo::Response::add_element(std::string ct, std::string ei, std::string en, unsigned long long int s, bool t) {
    struct DirListElement ele = {
        ct,
        ei,
        en,
        s,
        t
    };
    elements.push_back(ele);
}
void nar::Messagetypes::DirInfo::Request::send_mess(nar::Socket* skt){
    nlohmann::json dir_req_send;
    dir_req_send["header"] = sendHead();
    dir_req_send["payload"]["dir_name"] = dir;
    send_message(skt,dir_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json dir_req_recv = nlohmann::json::parse(temp);
    receive_message(dir_req_recv);
    return;
}
void nar::Messagetypes::DirInfo::Request::receive_message(nlohmann::json &dir_req_recv){
    nlohmann::json head = dir_req_recv["header"];
    recvFill(head);
    dir = dir_req_recv["payload"]["dir_name"];
    return;
}

nlohmann::json nar::Messagetypes::DirInfo::Request::test_json() {
    nlohmann::json dir_req_test;
    dir_req_test["header"] = sendHead();
    dir_req_test["payload"]["dir_name"] = dir;
    return dir_req_test;
}
void nar::Messagetypes::DirInfo::Response::send_mess(nar::Socket* skt){

    nlohmann::json dir_resp_send;
    dir_resp_send["header"] = sendHead();
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
    std::string temp = get_message(skt);
    nlohmann::json dir_resp_recv = nlohmann::json::parse(temp);
    receive_message(dir_resp_recv);
    return;
}
nlohmann::json nar::Messagetypes::DirInfo::Response::test_json(){

    nlohmann::json dir_resp_test;
    dir_resp_test["header"] = sendHead();
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

void nar::Messagetypes::DirInfo::Response::receive_message(nlohmann::json &dir_resp_recv){
    nlohmann::json head = dir_resp_recv["header"];
    recvFill(head);
    int size = dir_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++){
        std::string tchange_time = dir_resp_recv["payload"]["item_list"][i]["change_time"];
        std::string tentity_id = dir_resp_recv["payload"]["item_list"][i]["entity_id"];
        std::string tentity_name = dir_resp_recv["payload"]["item_list"][i]["entity_name"];
        unsigned long long int tentity_size =dir_resp_recv["payload"]["item_list"][i]["entity_size"];
        bool ttype = dir_resp_recv["payload"]["item_list"][i]["type"];
        this->add_element(tchange_time, tentity_id, tentity_name, tentity_size, ttype);
    }
    return;
}
