#include "FilePull.h"
std::string& nar::Messagetypes::FilePull::Request::get_filename() {
    return file_name;
}
std::string& nar::Messagetypes::FilePull::Request::get_dir() {
    return dir;
}
void nar::Messagetypes::FilePull::Request::send_mess(nar::Socket* skt) {
    nlohmann::json pull_req_send;
    pull_req_send["header"] = sendHead();
    pull_req_send["payload"]["file_name"] = this->file_name;
    pull_req_send["payload"]["dir"] = this->dir;
    send_message(skt,pull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json pull_req_recv = nlohmann::json::parse(temp);
    receive_message(pull_req_recv);
    return;
}
void nar::Messagetypes::FilePull::Request::receive_message(nlohmann::json pull_req_recv) {
    nlohmann::json head = pull_req_recv["header"];
    recvFill(head);
    this->file_name = pull_req_recv["payload"]["file_name"];
    this->dir = pull_req_recv["payload"]["dir"];
    return;
}

nlohmann::json nar::Messagetypes::FilePull::Request::test_json() {
    nlohmann::json pull_req_test;
    pull_req_test["header"] = sendHead();
    pull_req_test["payload"]["file_name"] = this->file_name;
    pull_req_test["payload"]["dir"] = this->dir;
    return pull_req_test;
}


void nar::Messagetypes::FilePull::Response::add_element(struct nar::Messagetypes::FilePull::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::Messagetypes::FilePull::Response::add_element(std::string mid, unsigned long long int cid, std::string sid, unsigned long long int csize) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize //chunk size
    };
    elements.push_back(ele);
}

std::vector<struct nar::Messagetypes::FilePull::Response::PeerListElement>& nar::Messagetypes::FilePull::Response::get_elements() {
    return elements;

}
unsigned short nar::Messagetypes::FilePull::Response::get_randevous_port() {
    return randevous_port;
}


void nar::Messagetypes::FilePull::Response::send_mess(nar::Socket* skt){
    nlohmann::json pull_resp_send;
    pull_resp_send["header"] = sendHead();
    pull_resp_send["payload"]["rand_port"] = this->randevous_port;
    pull_resp_send["payload"]["peer_list"] = nlohmann::json::array();
    pull_resp_send["payload"]["size"] = elements.size();
    for(int i = 0;i < elements.size();i++) {
        pull_resp_send["payload"]["peer_list"][i]["machine_id"] = elements[i].machine_id;
        pull_resp_send["payload"]["peer_list"][i]["chunk_id"] = elements[i].chunk_id;
        pull_resp_send["payload"]["peer_list"][i]["stream_id"] = elements[i].stream_id;
        pull_resp_send["payload"]["peer_list"][i]["chunk_size"] = elements[i].chunk_size;
    }
    send_message(skt,pull_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json pull_resp_recv = nlohmann::json::parse(temp);
    receive_message(pull_resp_secv);
    return;

}
void nar::Messagetypes::FilePull::Response::receive_message(nlohmann::json pull_resp_recv){
    nlohmann::json head = pull_resp_recv["header"];
    recvFill(head);
    this->randevous_port = pull_resp_recv["payload"]["rand_port"];
    unsigned long int size = pull_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++) {
        std::string mid = pull_resp_recv["payload"]["peer_list"][i]["machine_id"];
        unsigned long long int cid = pull_resp_recv["payload"]["peer_list"][i]["chunk_id"];
        std::string sid = pull_resp_recv["payload"]["peer_list"][i]["stream_id"];
        unsigned long long int csize = pull_resp_recv["payload"]["peer_list"][i]["chunk_size"];
        this->add_element(mid,cid,sid,csize);
    }
    return;
}

nlohmann::json nar::Messagetypes::FilePull::Response::test_json() {
    nlohmann::json pull_resp_test;
    pull_resp_test["header"] = sendHead();
    pull_resp_test["payload"]["rand_port"] = this->randevous_port;
    pull_resp_test["payload"]["peer_list"] = nlohmann::json::array();
    pull_resp_test["payload"]["size"] = elements.size();
    for(int i = 0;i < elements.size();i++) {
        pull_resp_test["payload"]["peer_list"][i]["machine_id"] = elements[i].machine_id;
        pull_resp_test["payload"]["peer_list"][i]["chunk_id"] = elements[i].chunk_id;
        pull_resp_test["payload"]["peer_list"][i]["stream_id"] = elements[i].stream_id;
        pull_resp_test["payload"]["peer_list"][i]["chunk_size"] = elements[i].chunk_size;
    }
    return pull_resp_test;
}
