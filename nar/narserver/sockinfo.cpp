#include <nar/narserver/sockinfo.h>

void nar::SockInfo::elevateKeepalive() {
    this->keepalive = true;
}

void nar::SockInfo::authenticate(const std::string& uname) {
    this->username = uname;
}

nar::Socket* nar::SockInfo::getSck() {
    return this->skt;
}

bool nar::SockInfo::isAuthenticated() {
    return this->username != std::string();
}

std::string nar::SockInfo::getAuthenticationHash() {
    return this->username;
}

bool nar::SockInfo::isKeepalive() {
    return this->keepalive;
}
