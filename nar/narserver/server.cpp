#include <map>
#include <nar/lib/Socket/Socket.h>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <mutex>
#include <atomic>
#include <nar/narserver/ServerGlobal.h>
#include <nar/narserver/Actions/ServerActions.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.h>

using namespace nlohmann;

std::map<std::string, bool> activetokens;


void handle_request(nar::Socket* skt, nar::ServerGlobal* s_global) {
    string msg = nar::trim(nar::get_message(*skt));
    string action = nar::Messaging::get_action(msg);
    if(action == "user_authentication_init") {
        nar::MessageTypes::UserAuthenticationInit::Request req;
        req.receive_message(msg);
        nar::ServerAction::authenticated_action(s_global, req, skt);
    } else if(action == "machine_authentication_init") {
    } else if(action == "user_register") {
        nar::MessageTypes::UserRegister::Request req;
        req.receive_message(msg);
        nar::ServerAction::register_action(s_global, req, skt);
    } else if(action == "machine_register") {
    }
}


void randezvous_thread(nar::ServerGlobal *s_global) {
    nar::USocket randezvous_socket(s_global->get_ioserv(), "127.0.0.1", 10000, 0);
    s_global->set_randezvous_port(randezvous_socket.get_port());
    s_global->set_next_stream_id(1);
    randezvous_socket.randezvous_server();
}

int main(int argc, char *argv[]) {
    std::srand(std::time(NULL));

    nar::ServerGlobal s_global(std::string("nar"), std::string("root"), std::string("123"));

    std::thread rand(&randezvous_thread, &s_global);
    rand.detach();

    nar::Socket entry_skt(s_global.get_ioserv(), 's');
    entry_skt.bind(16670, "127.0.0.1");

    while(true) {
        nar::Socket* new_skt = new nar::Socket(s_global.get_ioserv(), 'c');
        entry_skt.accept(*new_skt);

        std::thread thr(&handle_request, new_skt, &s_global);
        thr.detach();
    }
    return 0;
}
