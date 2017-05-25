#include "FilePull.h"
std::string& nar::MessageTypes::FilePull::Request::get_file_name() {
    return this->_file_name;
}
std::string& nar::MessageTypes::FilePull::Request::get_dir_name() {
    return this->_dir_name;
}
void nar::MessageTypes::FilePull::Request::send_mess(nar::Socket* skt ,nar::MessageTypes::FilePull::Response & resp) {
    nlohmann::json pull_req_send;
    pull_req_send["header"] = send_head();
    pull_req_send["payload"]["file_name"] = this->_file_name;
    pull_req_send["payload"]["dir_name"] = this->_dir_name;
    send_message(skt,pull_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json pull_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(pull_req_recv);
    return;
}
void nar::MessageTypes::FilePull::Request::receive_message(std::string& mess) {
    try {nlohmann::json pull_req_recv = nlohmann::json::parse(mess);
        nlohmann::json head = pull_req_recv["header"];
        recv_fill(head);
        this->_file_name = pull_req_recv["payload"]["file_name"];
        this->_dir_name = pull_req_recv["payload"]["dir_name"];
    } catch (...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("file pull bad message");
    }

    return;
}

nlohmann::json nar::MessageTypes::FilePull::Request::test_json() {
    nlohmann::json pull_req_test;
    pull_req_test["header"] = send_head();
    pull_req_test["payload"]["file_name"] = this->_file_name;
    pull_req_test["payload"]["dir_name"] = this->_dir_name;
    return pull_req_test;
}


void nar::MessageTypes::FilePull::Response::add_element(struct nar::MessageTypes::FilePull::Response::PeerListElement& ele) {
    _elements.push_back(ele);
}

void nar::MessageTypes::FilePull::Response::add_element(std::string mid, unsigned long long int cid, long long int sid, unsigned long long int csize,std::string hashed) {
    struct PeerListElement ele = {
        mid, //peer id
        cid, // chunk id
        sid, // stream id
        csize, //chunk size
        hashed
    };
    this->_elements.push_back(ele);
}

std::vector<struct nar::MessageTypes::FilePull::Response::PeerListElement>& nar::MessageTypes::FilePull::Response::get_elements() {
    return this->_elements;

}
unsigned short nar::MessageTypes::FilePull::Response::get_rendezvous_port() {
    return this->_rendezvous_port;
}

void nar::MessageTypes::FilePull::Response::set_rendezvous_port(unsigned short p) {
    _rendezvous_port = p;
}


void nar::MessageTypes::FilePull::Response::send_mess(nar::Socket* skt){
    int status = get_status_code();
    if(status == 200) {
        nlohmann::json pull_resp_send;
        pull_resp_send["header"] = send_head();
        pull_resp_send["payload"]["rand_port"] = this->_rendezvous_port;
        pull_resp_send["payload"]["peer_list"] = nlohmann::json::array();
        pull_resp_send["payload"]["size"] = _elements.size();
        for(int i = 0;i < _elements.size();i++) {
            pull_resp_send["payload"]["peer_list"][i]["machine_id"] = _elements[i].machine_id;
            pull_resp_send["payload"]["peer_list"][i]["chunk_id"] = _elements[i].chunk_id;
            pull_resp_send["payload"]["peer_list"][i]["stream_id"] = _elements[i].stream_id;
            pull_resp_send["payload"]["peer_list"][i]["chunk_size"] = _elements[i].chunk_size;
            pull_resp_send["payload"]["peer_list"][i]["hashed"] = _elements[i].hashed;
        }
        send_message(skt,pull_resp_send.dump());
        return;
    } else {
        nlohmann::json pull_resp_send;
        pull_resp_send["header"] = send_head();
        send_message(skt,pull_resp_send.dump());
        return;
    }


}
void nar::MessageTypes::FilePull::Response::receive_message(nlohmann::json pull_resp_recv){
    try {
        nlohmann::json head = pull_resp_recv["header"];
        recv_fill(head);
        
        int stat = get_status_code();
        if(stat/100 == 3) {
            throw nar::Exception::MessageTypes::BadRequest("Your request was not complete or was wrong", stat);
        } else  if(stat/100 == 4) {
            throw nar::Exception::MessageTypes::InternalServerDatabaseError("Database problem", stat);
        } else  if(stat/100 == 5) {
            throw nar::Exception::MessageTypes::InternalServerError("Some things went wrong in server", stat);
        }

        this->_rendezvous_port = pull_resp_recv["payload"]["rand_port"];
        unsigned long int size = pull_resp_recv["payload"]["size"];
        for(int i=0;i<size;i++) {
            std::string mid = pull_resp_recv["payload"]["peer_list"][i]["machine_id"];
            unsigned long long int cid = pull_resp_recv["payload"]["peer_list"][i]["chunk_id"];
            long long int sid = pull_resp_recv["payload"]["peer_list"][i]["stream_id"];
            unsigned long long int csize = pull_resp_recv["payload"]["peer_list"][i]["chunk_size"];
            std::string hashed = pull_resp_recv["payload"]["peer_list"][i]["hashed"];
            this->add_element(mid,cid,sid,csize,hashed);
        }


    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("file pull response recevei message");
    }
    return;
}

nlohmann::json nar::MessageTypes::FilePull::Response::test_json() {
    nlohmann::json pull_resp_test;
    pull_resp_test["header"] = send_head();
    pull_resp_test["payload"]["rand_port"] = this->_rendezvous_port;
    pull_resp_test["payload"]["peer_list"] = nlohmann::json::array();
    pull_resp_test["payload"]["size"] = _elements.size();
    for(int i = 0;i < _elements.size();i++) {
        pull_resp_test["payload"]["peer_list"][i]["machine_id"] = _elements[i].machine_id;
        pull_resp_test["payload"]["peer_list"][i]["chunk_id"] = _elements[i].chunk_id;
        pull_resp_test["payload"]["peer_list"][i]["stream_id"] = _elements[i].stream_id;
        pull_resp_test["payload"]["peer_list"][i]["chunk_size"] = _elements[i].chunk_size;

        pull_resp_test["payload"]["peer_list"][i]["hashed"] = _elements[i].hashed;
    }
    return pull_resp_test;
}
