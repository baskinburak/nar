
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


void randezvous_thread(nar::ServerGlobal *s_global) {
    nar::USocket randezvous_socket(s_global->get_ioserv(), "127.0.0.1", 10000, 0);
    s_global->set_randezvous_port(randevous_socket.get_port());
    s_global->set_next_stream_id(1);
    randevous_socket.randezvous_server();
}

int main(int argc, char *argv[]) {
    std::srand(std::time(NULL));

    nar::ServerGlobal s_global("nar", "root", "123");

    std::thread rand(&randezvous_thread, &s_global);
    rand.detach();

    nar::Socket entry_skt(s_global.io_service, 's');
    entry_skt.bind(12345);

    while(true) {
        nar::Socket* new_skt = new nar::Socket(s_global.get_ioserv(), 'c');
        entry_skt.accept(*new_skt, NULL);

        std::thread thr(&nar::server_receive, new_skt, &s_global);
        thr.detach();
    }
    return 0;
}
