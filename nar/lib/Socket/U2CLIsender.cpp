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

    std::string filename;
    cout << "filename: ";
    cin >> filename;

    nar::File file(filename, "r", false);
    cout << "here" << endl;
    unsigned long start, len;
    cin >> start >> len;
    cli_sck.send(file, start, len);
    return 0;
}
