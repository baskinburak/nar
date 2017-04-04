#include "USocket.h"
#include <iostream>
#include <nar/narnode/File/File.h>

using std::cin;
using std::cout;
using std::endl;
int main() {
    boost::asio::io_service io_serv;
    nar::USocket srv_sck(io_serv, "127.0.0.1", 12345, 0);

    std::cout << "bind port: " << srv_sck.get_port() << std::endl;

    srv_sck.randezvous_server();
    return 0;
}
