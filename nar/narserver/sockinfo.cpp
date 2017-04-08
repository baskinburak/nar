#include <nar/narserver/sockinfo.h>




nar::Socket* nar::SockInfo::get_sck() {
    return this->_skt;
}

nar::DBStruct::Machine& nar::SockInfo::get_machine() {
    return this->_machine;
}
