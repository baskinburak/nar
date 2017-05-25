#include "peers.h"
#include <nar/lib/Messaging/MessageTypes/KeepAliveCheck.h>
#include <nar/narserver/dbstructs.h>
void nar::Peers::read_start() {
    static int cnt = 0;
    std::cout << "read_start " << cnt++ << std::endl;
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::Peers::read_end() {
    static int cnt = 0;
    std::cout << "read_end " << cnt++ << std::endl;
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::Peers::write_start() {
    static int cnt = 0;
    std::cout << "write_start " << cnt++ << std::endl;
    write_mtx.lock();
}
void nar::Peers::write_end() {
    static int cnt = 0;
    std::cout << "write_end " << cnt++ << std::endl;
    write_mtx.unlock();
}

void nar::Peers::insert_keepalive(std::string& mac_id, nar::Socket* skt) {
    write_start();
    nar::DBStructs::Machine mac;

    try {
        mac = this->_db->getMachine(mac_id);
    } catch(sql::SQLException exp) {
        std::cout << exp.what() << std::endl;
        write_end();
        throw;
    }

    nar::DBStructs::Session sess;
    sess.machine_id = mac_id;
    unsigned long sessid = this->_db->insertSession(sess);
    nar::SockInfo* sck_inf = new nar::SockInfo(skt, mac, sessid);
    this->_keepalives[mac_id] = sck_inf;
    this->_macs.push_back(mac_id);
    write_end();
}
void nar::Peers::delete_keepalive(std::string& mac_id) {
    write_start();
    this->_keepalives.erase(mac_id);
    std::vector<std::string>::iterator it;
    if ( ( it = std::find(_macs.begin(), _macs.end(), mac_id) ) != _macs.end() ) {
       this->_macs.erase(it);
    }
    write_end();
}


nar::SockInfo* nar::Peers::peer_select(nar::DBStructs::User& user, unsigned long chunk_size) {
    read_start();
    try {
        nar::SockInfo* result = random_policy(user, chunk_size);
    } catch (...) {
        std::cout << "PEER SELECT FAILED" << std::endl;
        read_end();
        throw;
    }
    read_end();
    return result;
}

nar::SockInfo* nar::Peers::random_policy(nar::DBStructs::User& user, unsigned long chunk_size) {

    std::set<std::string> user_machines = _db->get_user_machines(user);

    bool another_keepalive = false;
    for(int i=0; i<_macs.size(); i++) {
        if(user_machines.find(_macs[i]) == user_machines.end()) {
            another_keepalive = true;
            break;
        }
    }

    if(!another_keepalive) {
        throw nar::Exception::Peers::NoValidPeer("No valid peer to push");
    }

    std::random_device rd; // only used once to initialise (seed) engine
    std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0,_keepalives.size()-1); // guaranteed unbiased
    std::set<std::string>::iterator it;
    std::string selected;
    nar::MessageTypes::KeepAliveCheck::Request req;
    nar::MessageTypes::KeepAliveCheck::Response resp;
	bool flg = true;
    do {
	    flg = true;
        auto random_integer = uni(rng);
        selected = _macs[random_integer % _keepalives.size()];
        nar::SockInfo* sckinf = _keepalives[selected];
        nar::Socket* sck = sckinf->get_sck();
        try {
            req.send_mess(sck, resp);
        } catch(...) {
            nar::SockInfo* sckinf = this->_keepalives[selected];
            unsigned long sessid = sckinf->get_sessid();
            nar::DBStructs::Session sess;
            sess.session_id = sessid;
            this->_db->leaveSession(sess);
            this->_keepalives.erase(selected);
            std::vector<std::string>::iterator it;
            if ( ( it = std::find(_macs.begin(), _macs.end(), selected) ) != _macs.end() ) {
               this->_macs.erase(it);
            }
		    flg = false;
       }
    } while((it = user_machines.find(selected)) != user_machines.end() || !flg);
    return this->_keepalives[selected];
}

nar::SockInfo* nar::Peers::get_peer(string& machine_id) {
    read_start();
    nar::SockInfo* result;
    std::map<string, nar::SockInfo*>::iterator it;
    it = _keepalives.find(machine_id);

    if (it == _keepalives.end())
        result = NULL;
    else{
        result = it->second;

        nar::MessageTypes::KeepAliveCheck::Request req;
        nar::MessageTypes::KeepAliveCheck::Response resp;
        nar::SockInfo* sckinf = it->second;
        nar::Socket* sck = sckinf->get_sck();
        try {
            req.send_mess(sck, resp);
        } catch(...) {
            unsigned long sessid = sckinf->get_sessid();
            nar::DBStructs::Session sess;
            sess.session_id = sessid;
            try{
                this->_db->leaveSession(sess);
            } catch(sql::SQLException & err) {
                std::cout<<err.what()<<std::endl;
            }

            this->_keepalives.erase(it->first);
            std::vector<std::string>::iterator iter;
            if ((iter = std::find(_macs.begin(), _macs.end(), machine_id)) != _macs.end()) {
                this->_macs.erase(iter);
            }
            result = NULL;
        }
    }

    read_end();
    return result;
}
