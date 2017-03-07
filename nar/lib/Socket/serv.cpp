#include "../Socket/Socket.h"
#include "../Exception/Exception.h"
#include <iostream>

using nar::Socket;
using std::cout;
using std::endl;



void test1() {      // test get_local_port
    boost::asio::io_service io_serv;
    Socket serv(io_serv,'s');
    Socket acc(io_serv,'c');
    serv.bind(12345);
    cout << "Port " << serv.get_local_port() << endl;
    serv.accept(acc);
    char *buf = new char(250);
    acc.recv(buf,250);
    cout << buf << endl;
    acc.send("biliyom kiiiiz \(*.*)/", 22);
    acc.close();
}


void test2() {      // test server sock
    boost::asio::io_service io_serv;
    Socket serv(io_serv,'s');
    Socket acc(io_serv,'c');
    serv.bind();
    cout << "Port " << serv.get_local_port() << endl;
    serv.accept(acc);
    char *buf = new char(250);
    acc.recv(buf,250);
    cout << buf << endl;
    acc.send("biliyom kiiiiz \(*.*)/", 22);
    acc.close();
}

void test3() {      // test server bind to specific interface                                                                                                                                                                                                                                               	
    boost::asio::io_service io_serv;
    Socket serv(io_serv,'s');
    Socket acc(io_serv,'c');
    serv.bind(12345,"127.0.0.1");
    cout << "Port " << serv.get_local_port() << endl;
    serv.accept(acc);
    char *buf = new char(250);
    acc.recv(buf,250);
    cout << buf << endl;
    acc.send("biliyom kiiiiz \(*.*)/", 22);
    cout << acc._sock->remote_endpoint().port() << endl;
    acc.close();
}


int main() {
    while(1) {
        try{
            test3();            
        }
        catch (const nar::Exception::ExcpBase e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}
