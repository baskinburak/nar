#include "FilePush.h"

std::string& nar::Messagetypes::FilePush::Request::get_filename() {
    return filename;
}

unsigned long long int nar::Messagetypes::FilePush::Request::get_filesize() {
    return filesize;
}

std::string& nar::Messagetypes::FilePush::Request::get_dir() {
    return dir;
}

void nar::Messagetypes::FilePush::Response::add_element(struct nar::Messagetypes::FilePush::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::Messagetypes::FilePush::Response::add_element(std::string mid, unsigned long long int cid, std::string sid, unsigned long long int csize) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize //chunk size
    };
    elements.push_back(ele);
}

std::vector<struct nar::Messagetypes::FilePush::Response::PeerListElement>& nar::Messagetypes::FilePush::Response::get_elements() {
    return elements;

}
unsigned short nar::Messagetypes::FilePush::Response::get_randevous_port() {
    return randevous_port;
}



void nar::Messagetypes::FilePush::Request::send_mess(nar::Socket* skt){
    nlohmann::json push_req_send;
    push_req_send["header"] = sendHead();
    push_req_send["payload"]["file_size"] = this->filesize;
    push_req_send["payload"]["dir"] = this->dir;
    push_req_send["payload"]["file_name"] = this->filename;
    send_message(skt,push_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json push_req_recv = nlohmann::json::parse(temp);
    receive_message(push_req_recv);
    return;
}
void nar::Messagetypes::FilePush::Request::receive_message(nlohmann::json push_req_recv){
    nlohmann::json head = push_req_recv["header"];
    recvFill(head);
    this->filesize = push_req_recv["payload"]["file_size"];
    this->dir = push_req_recv["payload"]["dir"];
    this->filename = push_req_recv["payload"]["file_name"];
    return;
}
nlohmann::json nar::Messagetypes::FilePush::Request::test_json() {
    nlohmann::json push_req_test;
    push_req_test["header"] = sendHead();
    push_req_test["payload"]["file_size"] = this->filesize;
    push_req_test["payload"]["dir"] = this->dir;
    push_req_test["payload"]["file_name"] = this->filename;
    return push_req_test;
}
void nar::Messagetypes::FilePush::Response::send_mess(nar::Socket* skt){
    nlohmann::json push_resp_send;
    push_resp_send["header"] = sendHead();
    push_resp_send["payload"]["rand_port"] = this->randevous_port;
    push_resp_send["payload"]["peer_list"] = nlohmann::json::array();
    push_resp_send["payload"]["size"] = elements.size();
    for(int i = 0;i < elements.size();i++) {
        push_resp_send["payload"]["peer_list"][i]["machine_id"] = elements[i].machine_id;
        push_resp_send["payload"]["peer_list"][i]["chunk_id"] = elements[i].chunk_id;
        push_resp_send["payload"]["peer_list"][i]["stream_id"] = elements[i].stream_id;
        push_resp_send["payload"]["peer_list"][i]["chunk_size"] = elements[i].chunk_size;
    }
    send_message(skt,push_resp_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json push_resp_recv = nlohmann::json::parse(temp);
    receive_message(push_resp_recv);
    return;

}
void nar::Messagetypes::FilePush::Response::receive_message(nlohmann::json push_resp_recv){
    nlohmann::json head = push_resp_recv["header"];
    recvFill(head);
    this->randevous_port = push_resp_recv["payload"]["rand_port"];
    unsigned long int size = push_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++) {
        std::string mid = push_resp_recv["payload"]["peer_list"][i]["machine_id"];
        unsigned long long int cid = push_resp_recv["payload"]["peer_list"][i]["chunk_id"];
        std::string sid = push_resp_recv["payload"]["peer_list"][i]["stream_id"];
        unsigned long long int csize = push_resp_recv["payload"]["peer_list"][i]["chunk_size"];
        this->add_element(mid,cid,sid,csize);
    }
    return;
}
nlohmann::json nar::Messagetypes::FilePush::Response::test_json() {
    nlohmann::json push_resp_test;
    push_resp_test["header"] = sendHead();
    push_resp_test["payload"]["rand_port"] = this->randevous_port;
    push_resp_test["payload"]["peer_list"] = nlohmann::json::array();
    push_resp_test["payload"]["size"] = elements.size();
    for(int i = 0;i < elements.size();i++) {
        push_resp_test["payload"]["peer_list"][i]["machine_id"] = elements[i].machine_id;
        push_resp_test["payload"]["peer_list"][i]["chunk_id"] = elements[i].chunk_id;
        push_resp_test["payload"]["peer_list"][i]["stream_id"] = elements[i].stream_id;
        push_resp_test["payload"]["peer_list"][i]["chunk_size"] = elements[i].chunk_size;
    }
    return push_resp_test;
}
