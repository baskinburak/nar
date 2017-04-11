#ifndef NAR_SOCKINFO_H
#define NAR_SOCKINFO_H
#include <nar/lib/Socket/Socket.h>
#include <nar/narserver/dbstructs.h>
namespace nar {
    class SockInfo {
        private:
            nar::DBStructs::Machine _machine;
            nar::Socket* _skt;
            unsigned long _sessid;
        public:
            SockInfo(nar::Socket* sk, nar::DBStructs::Machine& machine, unsigned long sessid): _skt(sk), _machine(machine), _sessid(sessid) {  }
            nar::Socket* get_sck();
            unsigned long get_sessid();
            nar::DBStructs::Machine& get_machine();
            std::string& get_machine_id();
    };
}

#endif
