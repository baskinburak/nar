#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <time.h>
namespace nar {

    void keep_alive(nar::Socket* sck) {
        while(!nar::DaemonTasks::handshake(sck, globals));
        nar::MessageTypes::KeepAlive::Request req;
        nar::MessageTypes::KeepAlive::Response resp(999);
        int tried = 5;
        while (tried--) {
            try {
                req.send(sck,resp);
            }
            catch (nar::Exception::MessageTypes::ServerSocketAuthenticationError exp) {
                std::cout << exp.what() << std::endl;
                sleep(1);
                continue;
            }
            break;
        }
        if(!tried) {
            std::cout << "Server may be down" << std::endl;
            exit(0);
        }
        return;
    }

    void chunk_push_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, std::string chunk_id) {
        nar::USocket peer_sock(stream_id);
        peer_sock.make_randevous(globals->get_narServerIp(), rand_port);

        char* buf = new char[chunk_size];
        nar::readdata(peer_sock, buf, chunk_size);
        std::string path(globals->get_narFolder() + std::string("/c"));
        path = path + chunk_id;
        std::cout << path << std::endl;
        int fd = nar::FileKeeper::openFdWrtonly( path.c_str());
        nar::FileKeeper::writeToFile(fd,  chunk_size, buf);

        peer_sock.stop_thread = true;
        usleep(1000000);
    }


    void daemon_dispatcher( Global *globals) {
        nar::Socket* server_socket = globals->establishServerConnection();
        keep_alive(server_socket);
        std::string message = nar::get_message( *server_socket);

        //resp.receive_message(transform(message))
        for(;;) {
            std::string action = find_message_action(message);
            if(action == std::string("wait_chunk_push_request")) {
                nar::MessageTypes::WaitChunkPush::Request req(0,0,0,0);
                nar::MessageTypes::WaitChunkPush::Response resp(999);
                req.receive_message(transform(message));

            }
        }
    }

};
