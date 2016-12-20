#ifndef NAR_HSOCKET_H
#define NAR_HSOCKET_H
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
/*
I---->
O<----

*/

namespace nar {
    class HSocket {
        private:
            int sockfd;
            struct sockaddr_in dst;
        public:
            HSocket() {
                if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
                    throw "HSock create error";
            }

            void establish(std::string ip, int port) {
                struct sockaddr_in dest;
                dest.sin_family = AF_INET;
                dest.sin_port = htons(port);
                inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
                dst = static_cast<struct sockaddr_in>(dest);
                int try_count = 1;
                bool initial_received = false;
                const char* initial = "I";
                char msg[64];
                struct sockaddr_in from;
                socklen_t len = sizeof(struct sockaddr);
                while(try_count--) {
                    ::sendto(sockfd, initial, 1/*len*/, 0/*flags*/, (struct sockaddr*)&dest, len);
                    usleep(1000);    
                }
                try_count = 50;
                while(try_count--) {
                    int res = recvfrom(sockfd, msg,1, MSG_DONTWAIT, (struct sockaddr*)&from, &len);
                    if(res > 0) {
                        if(msg[0] != 'I')
                            throw "establish fail";
                        break;
                    }
                    usleep(100000);
                }
		        std::cout << try_count << std::endl;
                if(try_count == -1)
                    throw "establish get message fail";
            }

            void sendto(std::string ip, int port, const char* buf, int len) {
                struct sockaddr_in dest;
                dest.sin_family = AF_INET;
                dest.sin_port = htons(port);
                inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
                ::sendto(sockfd, buf, len, 0, (struct sockaddr*)&dest, sizeof(struct sockaddr));
            }

            void send(std::string msg) {
                ::sendto(sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*) &dst, sizeof(dst));
            }

            std::string recv(int len) {
                char buf[len];
                struct sockaddr from;
                socklen_t socklen;
                int c = recvfrom(sockfd, buf, len, 0, &from, &socklen);
                if(c>0) {
                    return std::string(buf, c);
                }
                return std::string("");
            }
    };
}

#endif
