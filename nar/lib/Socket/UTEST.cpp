#include "U2Socket.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace std;
int main() {
    boost::asio::io_service io_service;
    nar::USocket sock(io_service, "127.0.0.1", 12345);
    while(true);
    return 0;
}
