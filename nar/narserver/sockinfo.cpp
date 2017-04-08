#include <nar/narserver/sockinfo.h>

void nar::SockInfo::elevateKeepalive() {
    this->keepalive = true;
}

void nar::SockInfo::authenticate(const std::string& uname,const std::string& macid) {
    this->username = uname;
    this->machineId = macid;
}

nar::Socket* nar::SockInfo::getSck() {
    return this->skt;
}

std::string nar::SockInfo::get_machine_id() {
    return this->machineId;
}

bool nar::SockInfo::isAuthenticated() {
    return this->username != std::string();
}
std::string nar::SockInfo::getUser() {
    return this->username;
}

std::string nar::SockInfo::getAuthenticationHash() {
    return this->machineId;
}

bool nar::SockInfo::isKeepalive() {
    return this->keepalive;
}
