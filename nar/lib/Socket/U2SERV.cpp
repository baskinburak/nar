#include "U2Socket.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main() {
    boost::asio::io_service io_serv;
    nar::USocket srv_sck(io_serv, "127.0.0.1", 12345, 0);

    srv_sck.randezvous_server();
    return 0;
}
