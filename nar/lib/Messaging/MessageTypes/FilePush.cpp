#include "FilePush.h"
#include <algorithm>

std::string& nar::MessageTypes::FilePush::Request::get_file_name() {
    return this->_file_name;
}

unsigned long long int nar::MessageTypes::FilePush::Request::get_file_size() {
    return this->_file_size;
}

std::string& nar::MessageTypes::FilePush::Request::get_dir_name() {
    return this->_dir_name;
}

void nar::MessageTypes::FilePush::Response::add_element(struct nar::MessageTypes::FilePush::Response::PeerListElement& ele) {
    this->_elements.push_back(ele);
}

void nar::MessageTypes::FilePush::Response::add_element(std::string mid, unsigned long long int cid, unsigned int sid, unsigned long long int csize) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize //chunk size
    };
    this->_elements.push_back(ele);
}

std::vector<struct nar::MessageTypes::FilePush::Response::PeerListElement>& nar::MessageTypes::FilePush::Response::get_elements() {
    return this->_elements;

}
unsigned short nar::MessageTypes::FilePush::Response::get_randezvous_port() {
    return this->_randezvous_port;
}



void nar::MessageTypes::FilePush::Request::send_mess(nar::Socket* skt , nar::MessageTypes::FilePush::Response & resp){
    nlohmann::json push_req_send;
    push_req_send["header"] = send_head();
    push_req_send["payload"]["file_size"] = this->_file_size;
    push_req_send["payload"]["dir_name"] = this->_dir_name;
    push_req_send["payload"]["file_name"] = this->_file_name;
    send_message(skt,push_req_send.dump());
    std::string temp = get_message(skt);
    resp.receive_message(temp);
    return;
}
void nar::MessageTypes::FilePush::Request::receive_message(std::string msg){
    nlohmann::json push_req_recv = nlohmann::json::parse(msg);
    nlohmann::json head = push_req_recv["header"];
    recv_fill(head);
    this->_file_size = push_req_recv["payload"]["file_size"];
    this->_dir_name = push_req_recv["payload"]["dir_name"];
    this->_file_name = push_req_recv["payload"]["file_name"];
    return;
}

nlohmann::json nar::MessageTypes::FilePush::Request::test_json() {
    nlohmann::json push_req_test;
    push_req_test["header"] = send_head();
    push_req_test["payload"]["file_size"] = this->_file_size;
    push_req_test["payload"]["dir_name"] = this->_dir_name;
    push_req_test["payload"]["file_name"] = this->_file_name;
    return push_req_test;
}
void nar::MessageTypes::FilePush::Response::send_mess(nar::Socket* skt){
    int status = get_status_code();
    if(status == 200) {
        nlohmann::json push_resp_send;
        push_resp_send["header"] = send_head();
        push_resp_send["payload"]["rand_port"] = this->_randezvous_port;
        push_resp_send["payload"]["peer_list"] = nlohmann::json::array();
        push_resp_send["payload"]["size"] = _elements.size();
        for(int i = 0;i < _elements.size();i++) {
            push_resp_send["payload"]["peer_list"][i]["machine_id"] = _elements[i].machine_id;
            push_resp_send["payload"]["peer_list"][i]["chunk_id"] = _elements[i].chunk_id;
            push_resp_send["payload"]["peer_list"][i]["stream_id"] = _elements[i].stream_id;
            push_resp_send["payload"]["peer_list"][i]["chunk_size"] = _elements[i].chunk_size;
        }
        send_message(skt,push_resp_send.dump());
        return;
    } else {
        nlohmann::json push_resp_send;
        push_resp_send["header"] = send_head();
        send_message(skt,push_resp_send.dump());
        return;
    }


}
void nar::MessageTypes::FilePush::Response::receive_message(std::string msg){
    nlohmann::json push_resp_recv = nlohmann::json::parse(msg);
    nlohmann::json head = push_resp_recv["header"];
    recv_fill(head);
    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::ServerSocketAuthenticationError("Server can not authenticate socket created for this user", _status_code);
    } else if(_status_code == 301) {
        throw nar::Exception::MessageTypes::NoValidPeerPush("Not enough valid peer for push operation", _status_code);
    }
    this->_randezvous_port = push_resp_recv["payload"]["rand_port"];
    unsigned long int size = push_resp_recv["payload"]["size"];
    for(int i=0;i<size;i++) {
        std::string mid = push_resp_recv["payload"]["peer_list"][i]["machine_id"];
        unsigned long long int cid = push_resp_recv["payload"]["peer_list"][i]["chunk_id"];
        unsigned int sid = push_resp_recv["payload"]["peer_list"][i]["stream_id"];
        unsigned long long int csize = push_resp_recv["payload"]["peer_list"][i]["chunk_size"];
        this->add_element(mid,cid,sid,csize);
    }
    sort(this->_elements.begin(), this->_elements.end());
    return;
}
nlohmann::json nar::MessageTypes::FilePush::Response::test_json() {
    nlohmann::json push_resp_test;
    push_resp_test["header"] = send_head();
    push_resp_test["payload"]["rand_port"] = this->_randezvous_port;
    push_resp_test["payload"]["peer_list"] = nlohmann::json::array();
    push_resp_test["payload"]["size"] = _elements.size();
    for(int i = 0;i < _elements.size();i++) {
        push_resp_test["payload"]["peer_list"][i]["machine_id"] = _elements[i].machine_id;
        push_resp_test["payload"]["peer_list"][i]["chunk_id"] = _elements[i].chunk_id;
        push_resp_test["payload"]["peer_list"][i]["stream_id"] = _elements[i].stream_id;
        push_resp_test["payload"]["peer_list"][i]["chunk_size"] = _elements[i].chunk_size;
    }
    return push_resp_test;
}
