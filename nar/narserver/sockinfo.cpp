#include <nar/narserver/sockinfo.h>

nar::Socket* nar::SockInfo::get_sck() {
    return this->_skt;
}

nar::DBStructs::Machine& nar::SockInfo::get_machine() {
    return this->_machine;
}

std::string& nar::SockInfo::get_machine_id() {
    return this->_machine.machine_id;
}

unsigned long nar::SockInfo::get_sessid() {
    return this->_sessid;
}
