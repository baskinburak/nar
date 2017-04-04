#include <iostream>
#include "reactive.h"
#include "global.h"
using std::cout;
using std::endl;


//g++ -std=c++11 tester.cpp ../utility.cpp File.cpp -lcrypto++ -lboost_system -lboost_iostreams -lboost_filesystem -lz -I/home/utku/nar  -o test

int main() {

    std::string confPath("/root/.nar/config");

    nar::Global* globalP = new nar::Global(confPath);
   // nar::reactive_dispatcher(globalP);

    return 0;
}
