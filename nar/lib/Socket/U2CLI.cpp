#include "U2Socket.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main() {
    boost::asio::io_service io_serv;

    unsigned short srv_port;
    cin >> srv_port;

    std::string type;
    cin >> type;


    nar::USocket cli_sck(io_serv, "127.0.0.1", srv_port, 7);

    cli_sck.connect();

    if(type=="cli") {
        nar::File file("denemesend.txt", "r");
        cli_sck.send(file);
    } else if(type=="serv") {
        nar::File file("deneme.txt", "w");
    }

    return 0;
}
