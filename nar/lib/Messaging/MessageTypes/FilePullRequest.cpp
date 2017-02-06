#include "FilePullRequest.h"
std::string& nar::messagetypes::FilePullRequest::Request::get_filename() {
    return file_name;
}
std::string& nar::messagetypes::FilePullRequest::Request::get_dir() {
    return dir;
}
void nar::messagetypes::FilePullRequest::Request::sendMessage() {
    nlohmann::json pull_req_send;
    pull_req_send["header"] = sendHead();
    pull_req_send["payload"]["file_name"] = this->file_name;
    pull_req_send["payload"]["dir"] = this->dir;
    std::cout<<pull_req_send.dump()<<std::endl;
    return;
}
void nar::messagetypes::FilePullRequest::Request::receiveMessage(nlohmann::json pull_req_recv) {
    nlohmann::json head = pull_req_recv["header"];
    recvFill(head);
    this->file_name = pull_req_recv["payload"]["file_name"];
    this->dir = pull_req_recv["payload"]["dir"];
    return;
}


void nar::messagetypes::FilePullRequest::Response::add_element(struct nar::messagetypes::FilePullRequest::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::messagetypes::FilePullRequest::Response::add_element(std::string mid, std::string cid, std::string sid, unsigned long long int csize) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize //chunk size
    };
    elements.push_back(ele);
}

std::vector<struct nar::messagetypes::FilePullRequest::Response::PeerListElement>& nar::messagetypes::FilePullRequest::Response::get_elements() {
    return elements;

}
unsigned short nar::messagetypes::FilePullRequest::Response::get_randevous_port() {
    return randevous_port;
}


void nar::messagetypes::FilePullRequest::Response::sendMessage(){
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
    std::cout<<pull_resp_send.dump()<<std::cout;
    return;

}
void nar::messagetypes::FilePullRequest::Response::receiveMessage(nlohmann::json pull_resp_recv){
    nlohmann::json head = pull_resp_recv["header"];
    recvFill(head);
    this->randevous_port = pull_resp_recv["payload"]["rand_port"];
    unsigned long int size = pull_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++) {
        std::string mid = pull_resp_recv["payload"]["peer_list"][i]["machine_id"];
        std::string cid = pull_resp_recv["payload"]["peer_list"][i]["chunk_id"];
        std::string sid = pull_resp_recv["payload"]["peer_list"][i]["stream_id"];
        unsigned long long int csize = pull_resp_recv["payload"]["peer_list"][i]["chunk_size"];
        this->add_element(mid,cid,sid,csize);
    }
    return;
}
