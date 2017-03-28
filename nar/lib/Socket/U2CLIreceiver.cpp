#include <iostream>
#include <nar/lib/Socket/U2Socket.h>
#include <nar/narnode/File/File.h>
#include <nar/lib/Exception/Exception.h>


using std::cin;
using std::cout;
using std::endl;
int main() {
    boost::asio::io_service io_serv;

    std::string srv_ip;
    unsigned short srv_port;
    cout << "server ip: ";
    cin >> srv_ip;
    cout << "server port: ";
    cin >> srv_port;

    unsigned int stream_id;
    cout << "stream id: ";
    cin >> stream_id;


    nar::USocket cli_sck(io_serv, srv_ip, srv_port, stream_id);

    cli_sck.connect();

    nar::File recvfile("recv.txt", "w", false);

    int total_read = 0;
    char buf[1024];
    while(total_read < 10891252) {
         int len = cli_sck.recv(buf, 1024);
         recvfile.write(buf, len);
         total_read += len;
    }
    return 0;
}
