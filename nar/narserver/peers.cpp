#include "peers.h"
void nar::Peers::read_start() {
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::Peers::read_end() {
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::Peers::write_start() {
    write_mtx.lock();
}
void nar::Peers::write_end() {
    write_mtx.unlock();
}

void nar::Peers::insert_keepalive(std::string& mac_id, nar::Socket* skt) {
    write_start();
    nar::DBStructs::Machine mac = this->_db->getMachine(mac_id);
    nar::SockInfo* sck_inf = new nar::SockInfo(skt,mac);
    this->_keepalives[mac_id] = sck_inf;
    write_end();
}
void nar::Peers::delete_keepalive(std::string& mac_id) {
    write_start();
    this->_keepalives.erase(mac_id);
    write_end();
}


nar::SockInfo* nar::Peers::peer_select(nar::DBStructs::User& user, unsigned long chunk_size) {
    read_start();
    nar::SockInfo* result = random_policy(user, chunk_size);
    read_end();
    return result;
}
nar::SockInfo* nar::Peers::random_policy(nar::DBStructs::User& user, unsigned long chunk_size) {
    std::set<std::string> user_machines = _db->get_user_machines(user);

    std::random_device rd; // only used once to initialise (seed) engine
    std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0,_keepalives.size()-1); // guaranteed unbiased
    std::set<std::string>::iterator it;
    std::string selected ;
    do {
        auto random_integer = uni(rng);
        selected = _macs[random_integer];
    }
    while((it = user_machines.find(selected)) != user_machines.end());
    return _keepalives[selected];
}
