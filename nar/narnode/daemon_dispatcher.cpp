#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <time.h>
namespace nar {

    void keep_alive(nar::Socket* sck) {
        while(!nar::DaemonTasks::handshake(sck, globals));
        nar::MessageTypes::KeepAlive::Request req;
        nar::MessageTypes::KeepAlive::Response resp;
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




    void chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id) {
        nar::USocket cli_sck(global->io_serv, global->get_narServerIp(), rand_port, stream_id);
        cli_sck.connect();

        std::string path(globals->get_narFolder() + std::string("/c"));
        path = path + chunk_id;
        nar::File recvfile(path, "w", false);

        int total_read = 0;
        char buf[1024];
        while(total_read < chunk_size) {
             int len = cli_sck.recv(buf, 1024);
             recvfile.write(buf, len);
             total_read += len;
        }
        return;
    }


    void chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, std::string chunk_id) {
        nar::USocket cli_sck(globals->io_serv, globals->get_narServerIp(), rand_port, stream_id);
        cli_sck.connect();
        std::string path(globals->get_narFolder() + std::string("/c"));
        nar::File f( (path+chunk_id ).c_str(), 'r', false);
        cli_sck.send(f,0,f.size());
    }

    void daemon_dispatcher( Global *globals) {
        nar::Socket* server_socket = globals->establishServerConnection();
        keep_alive(server_socket);
        std::string message = nar::get_message( *server_socket);

        //resp.receive_message(transform(message))
        for(;;) {

            std::string action = find_message_action(message);

            if(action == std::string("wait_chunk_push_request")) {
                nar::MessageTypes::WaitChunkPush::Request req;
                req.receive_message(transform(message));

                long long int stream_id = req.get_stream_id();
                long long int chunk_id = req.get_chunk_id();
                long long int chunk_size = req.get_chunk_size();
                unsigned short rand_port = req.get_randevous_port();

                // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

                nar::MessageTypes::WaitChunkPush::Response resp;
                resp.send_mess(server_socket);

                std::thread thr(&chunk_push_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
                thr.detach();
            } else if (action == std::string("wait_chunk_pull_request")) {
              nar::MessageTypes::WaitChunkPull::Request req;
              req.receive_message(transform(message));
              long long int stream_id = req.get_stream_id();
              long long int chunk_id = req.get_chunk_id();
              long long int chunk_size = req.get_chunk_size();
              unsigned short rand_port = req.get_randevous_port();

              // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

              nar::MessageTypes::WaitChunkPull::Response resp;
              resp.send_mess(server_socket);

              std::thread thr(&chunk_pull_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
              thr.detach();
            }
        }
    }

};
