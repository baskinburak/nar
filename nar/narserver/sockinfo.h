#ifndef NAR_SOCKINFO_H
#define NAR_SOCKINFO_H
#include <nar/lib/Socket/Socket.h>

namespace nar {
    class SockInfo {
        private:
            std::string machineId;
            nar::Socket* skt;
        public:
            SockInfo(nar::Socket* sk): skt(sk) {  }
            void elevateKeepalive();
            void authenticate(const std::string& uname, const std::string& macid);
            nar::Socket* getSck();
            bool isAuthenticated();
            std::string getAuthenticationHash();
            std::string getUser();
            bool isKeepalive();
    };
}

#endif
