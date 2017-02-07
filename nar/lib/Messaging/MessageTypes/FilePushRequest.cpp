#include "FilePushRequest.h"

std::string& nar::messagetypes::FilePushRequest::Request::get_filename() {
    return filename;
}

unsigned long long int nar::messagetypes::FilePushRequest::Request::get_filesize() {
    return filesize;
}

std::string& nar::messagetypes::FilePushRequest::Request::get_dir() {
    return dir;
}

void nar::messagetypes::FilePushRequest::Response::add_element(struct nar::messagetypes::FilePushRequest::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::messagetypes::FilePushRequest::Response::add_element(std::string mid, std::string cid, std::string sid, unsigned long long int csize) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize //chunk size
    };
    elements.push_back(ele);
}

std::vector<struct nar::messagetypes::FilePushRequest::Response::PeerListElement>& nar::messagetypes::FilePushRequest::Response::get_elements() {
    return elements;

}
unsigned short nar::messagetypes::FilePushRequest::Response::get_randevous_port() {
    return randevous_port;
}



void nar::messagetypes::FilePushRequest::Request::sendMessage(){
    nlohmann::json push_req_send;
    push_req_send["header"] = sendHead();
    push_req_send["payload"]["file_size"] = this->filesize;
    push_req_send["payload"]["dir"] = this->dir;
    push_req_send["payload"]["file_name"] = this->filename;
    std::cout<<push_req_send.dump()<<std::endl;
    return;
}
void nar::messagetypes::FilePushRequest::Request::receiveMessage(nlohmann::json push_req_recv){
    nlohmann::json head = push_req_recv["header"];
    recvFill(head);
    this->filesize = push_req_recv["payload"]["file_size"];
    this->dir = push_req_recv["payload"]["dir"];
    this->filename = push_req_recv["payload"]["file_name"];
    return;
}
nlohmann::json nar::messagetypes::FilePushRequest::Request::test_json() {
    nlohmann::json push_req_test;
    push_req_test["header"] = sendHead();
    push_req_test["payload"]["file_size"] = this->filesize;
    push_req_test["payload"]["dir"] = this->dir;
    push_req_test["payload"]["file_name"] = this->filename;
    return push_req_test;
}
void nar::messagetypes::FilePushRequest::Response::sendMessage(){
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
    std::cout<<push_resp_send.dump()<<std::cout;
    return;

}
void nar::messagetypes::FilePushRequest::Response::receiveMessage(nlohmann::json push_resp_recv){
    nlohmann::json head = push_resp_recv["header"];
    recvFill(head);
    this->randevous_port = push_resp_recv["payload"]["rand_port"];
    unsigned long int size = push_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++) {
        std::string mid = push_resp_recv["payload"]["peer_list"][i]["machine_id"];
        std::string cid = push_resp_recv["payload"]["peer_list"][i]["chunk_id"];
        std::string sid = push_resp_recv["payload"]["peer_list"][i]["stream_id"];
        unsigned long long int csize = push_resp_recv["payload"]["peer_list"][i]["chunk_size"];
        this->add_element(mid,cid,sid,csize);
    }
    return;
}
nlohmann::json nar::messagetypes::FilePushRequest::Response::test_json() {
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
