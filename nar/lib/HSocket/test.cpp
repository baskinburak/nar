#include "HSocket.h"
#include <iostream>
int main() {
    nar::HSocket sock;
    sock.sendto(std::string("144.122.238.149"), 54321, "HALLOLENA", 9);

    std::string ip;
    int port;
    std::cout << "ip port: ";
    std::cin >> ip >> port;
    sock.establish(ip, port);
	usleep(2000000);
    int send = 1;
    //std::cout << "sender(1)/receiver(0): ";
    //std::cin >> send;
    if(send)
        sock.send(std::string("slm kardis"));
    else {
        std::string res = sock.recv(100);
        std::cout << res << std::endl;
    }
}
