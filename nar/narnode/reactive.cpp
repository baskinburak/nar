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
#include <stdio.h>

void nar::keep_alive(nar::Socket* sck, nar::Global* globals) {
    std::string macid = globals->get_machine_id();
    nar::MessageTypes::KeepAlive::Request req(macid);
    nar::MessageTypes::KeepAlive::Response resp;
    int tried = 1;

    std::cout << "Trying to open keepalive connection to the server: ";
    while (true) {
        std::cout << tried++ << " ";
        try {
            req.send_mess(sck, resp);
            break;
        } catch (nar::Exception::MessageTypes::ServerSocketAuthenticationError& exp) {
            sleep(1);
        } catch (nar::Exception::MessageTypes::BadMessageReceive& exp) {
            std::cout << "Are you sure you are connecting to the server?" << std::endl << "We have received a badly constructed response." << std::endl;
            exit(0);
        }
    }
    std::cout << "Connected." << std::endl;
    return;
}




void nar::chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket* cli_sck = new nar::USocket(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    //std::cout << "HERE THREAD, chunk size: "<< chunk_size << std::endl;
    cli_sck->connect();
    //std::cout << "Ready To Read" << std::endl;


    std::string path(globals->get_file_folder() + std::string("/c"));
    path = path + std::to_string(chunk_id);
    nar::File recvfile(path, "w", false);

    int total_read = 0;
    char buf[1024];
    while(total_read < chunk_size) {
        //std::cout << stream_id << " before" << std::endl;
         int len = cli_sck->recv(buf, 1024);
        //std::cout << stream_id << " after " << len << std::endl;
         recvfile.write(buf, len);
         total_read += len;
    }
    recvfile.close();
    cli_sck->close();
    return;
}


void nar::chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket* cli_sck = new nar::USocket(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    cli_sck->connect();
    std::string path(globals->get_file_folder() + std::string("/c"));
    nar::File f( (path+std::to_string(chunk_id) ).c_str(), "r", false);
    cli_sck->send(f,0,f.size());
    cli_sck->close();
}

bool nar::delete_chunk(unsigned int stream_id, nar::Global* globals, unsigned short rand_port, long long int chunk_id)  {
	std::string chunkToDelete = std::string("c") + std::to_string(chunk_id);
	std::cout << "chunk to delete: " << chunkToDelete << std::endl;

	boost::filesystem::path dir(chunkToDelete);
    try {
        dir = boost::filesystem::canonical(dir);
    } catch (boost::filesystem::filesystem_error er) {
        std::cout << er.what() << '\n';
        return false;
    }
	/*
	boost::filesystem::path current_path(boost::filesystem::current_path());
	std::cout << "Current path is : " << current_path << std::endl;
	std::string full_path = current_path.string() + chunkToDelete;
	std::cout << "Full path is : " << full_path << std::endl;
	*/
	std::cout << "Full path is : " << dir.string() << std::endl;
	
	//exists, so delete it!
	int status;
	status = boost::filesystem::remove(dir);
	// status == number of files deleted.
	if (status == 1) {
		return true;
	}
}

void nar::reactive_dispatcher(nar::Global *globals) {
    nar::Socket* server_socket = globals->establish_server_connection();
    keep_alive(server_socket, globals);


    //resp.receive_message(transform(message))
    for(;;) {
       // std::cout<<"MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" <<std::endl;
        std::string message = nar::get_message( *server_socket);
       // std::cout<<"MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" <<message<<std::endl;
        std::string action = Messaging::get_action(message);
        std::cout << "Action::    " <<  std::endl << action << std::endl;
        if(action == std::string("wait_chunk_push_request")) {
            nar::MessageTypes::WaitChunkPush::Request req;
            req.receive_message(Messaging::transform(message));

            long long int stream_id = req.get_stream_id();
            long long int chunk_id = req.get_chunk_id();
            long long int chunk_size = req.get_chunk_size();
            unsigned short rand_port = req.get_randevous_port();

         //   std::cout << "STREAM ID: ************ " << stream_id << std::endl;

            // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

            nar::MessageTypes::WaitChunkPush::Response resp(200);
            resp.send_mess(server_socket);

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
        }
    }
}
