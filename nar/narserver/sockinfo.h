#ifndef NAR_SOCKINFO_H
#define NAR_SOCKINFO_H
#include <nar/lib/Socket/Socket.h>
#include <nar/narserver/dbstructs.h>
namespace nar {
    class SockInfo {
        private:
            nar::DBStructs::Machine _machine;
            nar::Socket* _skt;
        public:
            SockInfo(nar::Socket* sk, nar::DBStructs::Machine& machine): _skt(sk), _machine(machine) {  }
            nar::Socket* get_sck();
            nar::DBStructs::Machine& get_machine();
    };
}

#endif
