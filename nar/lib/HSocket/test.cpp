#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <algorithm>
#include "THSocket.h"

int main(int argc, char* argv[]) {
    nar::THSocket sck;
    sck.introduce(argv[1], "localhost", "12346");
    int a;
    std::cin >> a;
    if(argc > 2)
        sck.establish(argv[2]);
    else {
        sck.wait();
    }
    //sck.establish(std::string("necmi"), std::string("baskin"), std::string("localhost"), std::string("12345"));
    return 0;
}
