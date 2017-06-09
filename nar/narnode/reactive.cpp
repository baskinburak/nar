#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <time.h>
#include "reactive.h"
#include <thread>
#include <string>
#include <nar/lib/Messaging/MessageTypes/KeepAliveCheck.h>
#include <nar/lib/Messaging/MessageTypes/DeleteMachineChunk.h>
#include <stdio.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

void nar::keep_alive(nar::Socket* sck, nar::Global* globals) {
    std::string macid = globals->get_machine_id();
    nar::MessageTypes::KeepAlive::Request req(macid);
    nar::MessageTypes::KeepAlive::Response resp;
    int tried = 1;

    //std::cout << "Trying to open keepalive connection to the server: ";
    while (true) {
        std::cout << tried++ << " ";
        try {
            req.send_mess(sck, resp);
            std::cout << resp.get_status_code() << std::endl;
            break;
        } catch (nar::Exception::MessageTypes::ServerSocketAuthenticationError& exp) {
            NAR_LOG << "You probably have non-existing machine id" << std::endl;
            sleep(1);
        } catch (nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "Are you sure you are connecting to the server?" << std::endl << "We have received a badly constructed response." << std::endl;
            exit(0);
        }
    }
    std::vector<std::string> &delete_list = resp.get_delete_list();
    if(delete_list.size()>0) {
        for(int i=0;i<delete_list.size();i++) {
            delete_chunk_keepalive(globals,delete_list[i]);
        }
    }
    std::cout << "Connected." << std::endl;
    return;
}




void nar::chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket* cli_sck = new nar::USocket(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    cli_sck->connect();


    boost::filesystem::path path(globals->get_file_folder());
    path /= std::string("c") + std::to_string(chunk_id);

    nar::File recvfile(path.string(), "w", false);

    int total_read = 0;
    char buf[1024];

    byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];
    CryptoPP::Weak::MD5 hash;

    while(total_read < chunk_size) {
         int len;
         try {
            len = cli_sck->recv(buf, 1024);
         } catch(nar::Exception::USocket::InactivePeer& exp) {
            std::cout << "Reactive push replier peer inactive. Removing file: " << path << std::endl;
            boost::filesystem::remove(path);
            return;
         }
         hash.Update((const byte*) buf, len);
         recvfile.write(buf, len);
         total_read += len;
    }
    recvfile.close();
    cli_sck->close();

    CryptoPP::HexEncoder encoder;
	std::string hash_string;               // Hash of the received chunk

	encoder.Attach( new CryptoPP::StringSink( hash_string ) );
	encoder.Put( digest, sizeof(digest) );
	encoder.MessageEnd();

    return;
}


void nar::chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket* cli_sck = new nar::USocket(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    cli_sck->connect();
    boost::filesystem::path path(globals->get_file_folder());
    path /= std::string("c");
    nar::File f( (path.string()+std::to_string(chunk_id) ).c_str(), "r", false);
    std::string hash;
    cli_sck->send(f,0,f.size(), hash);
    cli_sck->close();
}

void nar::delete_chunk_keepalive( nar::Global* globals, std::string chunk_id)  {
    std::string chunkToDelete = std::string("c").append(chunk_id);
    std::cout << "Chunk Name: " << chunkToDelete  << " is being deleted." << std::endl;

    boost::filesystem::path path(globals->get_file_folder());
    path /= chunkToDelete;

    if( !boost::filesystem::exists(path) || boost::filesystem::is_directory(path) || !boost::filesystem::remove(path) ) {
        std::cout << "Reactive delete failed" << std::endl;
    }
    return;
}

void nar::delete_chunk( nar::Global* globals, std::string chunk_id, nar::Socket* server_socket)  {
	std::string chunkToDelete = std::string("c").append(chunk_id);
	std::cout << "Chunk Name: " << chunkToDelete  << " is being deleted." << std::endl;

    boost::filesystem::path path(globals->get_file_folder());
    path /= chunkToDelete;

    if( !boost::filesystem::exists(path) || boost::filesystem::is_directory(path) || !boost::filesystem::remove(path) ) {
        nar::MessageTypes::DeleteMachineChunk::Response resp(601);
        std::cout << "Reactive delete failed" << std::endl;
        try {
            resp.send_mess(server_socket);
        }
        catch (...) { std::cout << "Daemon-server socket failed." << std::endl;}
        return;
    }
    nar::MessageTypes::DeleteMachineChunk::Response resp(200);
    try {
        std::cout << "Resp send b4" << std::endl;
        resp.send_mess(server_socket);
        std::cout << "Resp send After" << std::endl;
    }
    catch (...) { std::cout << "Daemon-server socket failed." << std::endl; }
    return;
}

void nar::reactive_dispatcher(nar::Global *globals) {
    nar::Socket* server_socket = globals->establish_server_connection();
    NAR_LOG << "Server Connection established" << std::endl;
    keep_alive(server_socket, globals);
    NAR_LOG << "KeepAlive Connection established" << std::endl;

    for(;;) {
        std::string message;
        try {
            message = nar::get_message( *server_socket);
        } catch(...) {
            NAR_LOG << "Server Connection Lost, trying to reconnect ... " << std::endl;
            server_socket->forceclose();
            delete server_socket;
            sleep(1);
            server_socket = globals->establish_server_connection();
            keep_alive(server_socket, globals);
            continue;
        }
        std::string action;
        try {
            action = Messaging::get_action(message);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "Are you sure you are connecting to the server?" << std::endl << "We have received a badly constructed response." << std::endl;
            exit(0);
        }

        if(action == std::string("wait_chunk_push_request")) {
            nar::MessageTypes::WaitChunkPush::Request req;

            try {
                req.receive_message(Messaging::transform(message));
            } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
                std::cout << "Are you sure you are connecting to the server?" << std::endl << "We have received a badly constructed response." << std::endl;
                exit(0);
            }

            long long int stream_id = req.get_stream_id();
            long long int chunk_id = req.get_chunk_id();
            long long int chunk_size = req.get_chunk_size();
            unsigned short rand_port = req.get_randevous_port();

            // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

            nar::MessageTypes::WaitChunkPush::Response resp(200);
            try {
                resp.send_mess(server_socket);
            } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow& exp) {
                continue;
            } catch(nar::Exception::LowLevelMessaging::FormatError& exp) {
                continue;
            } catch(nar::Exception::LowLevelMessaging::ServerSizeIntOverflow& exp) {
                continue;
            } catch(nar::Exception::Socket::SystemError& exp) {
                continue;
            } catch(...) {
                continue;
            }

            std::thread thr(&nar::chunk_push_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
            thr.detach();
        } else if (action == std::string("wait_chunk_pull_request")) {
          nar::MessageTypes::WaitChunkPull::Request req;
          req.receive_message(Messaging::transform(message));
          unsigned int stream_id = req.get_stream_id();
          long long int chunk_id = req.get_chunk_id();
          int chunk_size = req.get_chunk_size();
          unsigned short rand_port = req.get_randevous_port();

          // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

          nar::MessageTypes::WaitChunkPull::Response resp(200);
          resp.send_mess(server_socket);

          std::thread thr(nar::chunk_pull_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
          thr.detach();
        } else if(action == std::string("keepalive_check")) {
            nar::MessageTypes::KeepAliveCheck::Response resp(200);
            resp.send_mess(server_socket);
        } else if(action == std::string("delete_machine_chunk") ) {
            nar::MessageTypes::DeleteMachineChunk::Request req;
            try {
                req.receive_message(message);
            } catch( nar::Exception::MessageTypes::BadMessageReceive& e ) {
                std::cout << e.what() << std::endl;
                nar::MessageTypes::DeleteMachineChunk::Response resp(600);
                try {
                    resp.send_mess(server_socket);
                } catch (...) {}
                continue;
            }
            std::string c_id = req.get_chunk_id();
            std::thread thr(&delete_chunk, globals, c_id, server_socket);
            thr.join();
        }
    }
}
