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
#include <nar/lib/Messaging/MessageTypes/DaemonShutdown.h>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/lib/Logging/logger.h>



using namespace nlohmann;

nar::Logger* nar_log = new nar::Logger(std::cout);      // LOGGER OBJECT


void handle_request(nar::Socket* skt, nar::ServerGlobal* s_global) {
    try {
        string msg = nar::get_message(*skt);
        string action = nar::Messaging::get_action(msg);
        if(action == "user_authentication_init") {
            nar::MessageTypes::UserAuthenticationInit::Request req;
            req.receive_message(msg);
            NAR_LOG << "Authentication init has received" << std::endl;
            nar::ServerAction::authenticate_action(s_global, req, skt);
        } else if(action == "user_register") {
            nar::MessageTypes::UserRegister::Request req;
            req.receive_message(msg);
            nar::ServerAction::register_action(s_global, req, skt);
        } else if(action == "keepalive") {
            nar::MessageTypes::KeepAlive::Request req;
            try {
                req.receive_message(msg);
            } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
                nar::MessageTypes::KeepAlive::Response resp(301);
                resp.send_mess(skt);
                skt->close();
                return;
            }
            nar::ServerAction::keepalive_action(s_global, req, skt);
        } else if(action == "daemon_shutdown") {
            nar::MessageTypes::DaemonShutdown::Request req;

            try{
                req.receive_message(msg);
            } catch(nar::Exception::MessageTypes::BadMessageReceive & e) {
                std::cout<< e.what()<<std::endl;
                return;
            }

            nar::ServerAction::daemon_shutdown_action(s_global,req,skt);
        }
    } catch(nar::Exception::Socket::SystemError& exp) {
        skt->forceclose();
        return;
    } catch(nar::Exception::LowLevelMessaging::NoSize& exp) {
        skt->forceclose();
        return;
    } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow& exp) {
        skt->forceclose();
        return;
    } catch(nar::Exception::LowLevelMessaging::ServerSizeIntOverflow& exp) {
        skt->forceclose();
        return;
    } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
        skt->forceclose();
        return;
    } catch(...) {
        skt->forceclose();
        return;
    }

    skt->close();
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

    nar::Socket entry_skt(s_global.get_ioserv(), s_global.get_ctx(), 's');
    entry_skt.bind(16670);

    while(true) {
        nar::Socket* new_skt = new nar::Socket(s_global.get_ioserv(), s_global.get_ctx(), 'c');
        entry_skt.accept(*new_skt);
        NAR_LOG << "New Connection" << std::endl;
        std::thread thr(&handle_request, new_skt, &s_global);
        thr.detach();
    }
    return 0;
}
