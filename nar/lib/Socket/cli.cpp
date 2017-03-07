#include "../Socket/Socket.h"
#include "../Exception/Exception.h"
#include <iostream>

using nar::Socket;
using std::cout;
using std::endl;


void test1() {      // test bind(port) and get_ip
    boost::asio::io_service io_serv;
    Socket cli(io_serv,'c');
    cli.connect(std::string("127.0.0.1"),12345);
    cli.send("Fatih var ya hayvan :)", 22);
    cout << "Ip: " << cli.get_dest_ip() << endl;
    char *buf = new char(250);
    cli.recv(buf,250);
    cout << buf << endl;
    cli.close();
}


void test2() {      // test bind(void)
    boost::asio::io_service io_serv;
    Socket cli(io_serv,'c');
    unsigned short port;
    std::cin >> port;
    cli.connect(std::string("127.0.0.1"),port);
    cli.send("Fatih var ya hayvan :)", 22);

    char *buf = new char(250);
    cli.recv(buf,250);
    cout << buf << endl;
    cli.close();
}

void test3() {
    boost::asio::io_service io_serv;
    Socket cli(io_serv,'c');
    unsigned short port;
    std::cin >> port;
    cli.bind(66879,"127.0.0.1")
    cli.connect(std::string("127.0.0.1"),port);
    cli.send("Fatih var ya hayvan :)", 22);

    char *buf = new char(250);
    cli.recv(buf,250);
    cout << buf << endl;
    cli.close();
}


int main() {
    try{
        test3();
    }
    catch (const nar::Exception::ExcpBase e) {
        cout << e.what() << endl;
    }
    return 0;
}
