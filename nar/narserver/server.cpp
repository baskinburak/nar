
#include <map>
#include <nar/lib/Socket/Socket.h>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include <nar/narnode/utility.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/sockinfo.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <mutex>
#include <atomic>

#include <nar/lib/Messaging/server_receive.h>
#include <nar/narserver/ServerGlobal.h>

using namespace nlohmann;

std::map<std::string, bool> activetokens;
std::atomic<unsigned short> randevous_port;
std::atomic<unsigned int> stream_id;


void randevous_thread(nar::ServerGlobal *s_global) {
    nar::USocket randevous_socket(0U);
    s_global->randevous_port = randevous_socket.get_port();
    randevous_socket.randevous_server();
}

int main(int argc, char *argv[])
{
    stream_id = 1;
    std::srand(std::time(NULL));

    nar::ServerGlobal s_global;

    std::thread rand(&randevous_thread,&s_global);
    rand.detach();

    nar::Socket entry_skt(s_global.io_service, 's');
    entry_skt.bind(12345);

    while(true) {
        nar::Socket* new_skt = new nar::Socket(s_global.io_service, 'c');
        entry_skt.accept(*new_skt, NULL);

        std::thread thr(&nar::server_receive, new_skt,&s_global);
        thr.detach();
    }
    return 0;
}
