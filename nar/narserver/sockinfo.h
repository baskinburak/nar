#ifndef NAR_SOCKINFO_H
#define NAR_SOCKINFO_H
#include <nar/lib/Socket/Socket.h>

namespace nar {
    class SockInfo {
        private:
            std::string username;
            nar::Socket* skt;
            bool keepalive;
        public:
            SockInfo(nar::Socket* sk): skt(sk) {  }
            void elevateKeepalive();
            void authenticate(const std::string& uname);
            nar::Socket* getSck();
            bool isAuthenticated();
            std::string getAuthenticationHash();
            bool isKeepalive();
    };
}

#endif
