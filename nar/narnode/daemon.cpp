#include <nar/narnode/IPC/ipcserver.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <nar/narnode/Exception/exception.h>
#include <nar/narnode/utility.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <cstdlib>
#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/Task/LS.h>
#include <nar/narnode/Task/PullFile.h>
#include <nar/narnode/global.h>
#include <nar/lib/nlohJson/json.hpp>
#include <utility>

void handle_cli_ipc(int sockfd, nar::Global* globals) {
    char buf[129];
    int len = nar::get_int_sckt(sockfd);
    nar::send_string_sckt(sockfd, std::string("OK"), 2);
    std::string json = nar::get_string_sckt(sockfd, len);

    std::cout << json << std::endl;

    rapidjson::Document doc;
    doc.Parse(json.c_str());
    std::string action(doc["action"].GetString());


    if(action == std::string("push")) {
        nar::task::PushFile task(doc["file"].GetString());
        task.run(sockfd, globals);
    } else if(action == std::string("ls")) {
        nar::task::LS task;
        task.run(sockfd, globals);
    } else if(action == std::string("pull")) { //todo
        nar::task::PullFile task(doc["file"].GetString());
        task.run(sockfd, globals);
    }

    close(sockfd);
}

void setServerConnection(nar::Socket &skt, nar::Global *globals){
	
	skt.create();
	skt.connect(globals->get_narServerIp(),globals->get_narServerPort());
	
	while(!nar::task::ITask::handshake(skt, globals));

	nar::send_int_sckt(skt.getSckDescriptor(), 51);
	std::string str(" {\"header\":{\"action\": \"keepalive\",\"channel\": \"ps\"}}");
	nar::send_string_sckt(skt.getSckDescriptor(), str, str.size()); 	// KeepAlive Message
	nar::get_message( skt);
	
	return;
}

std::pair < nar::Socket *, int >  wait_chunk(){
	
	nar::Socket *tmp = new nar::Socket();
	/*int m_sock = socket ( AF_INET,SOCK_STREAM, 0 );
	int on = 1;
	setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) );*/
	int port = tmp->bind(0);

	tmp->listen();
	return std::make_pair(tmp,port);
}

void keepAlive( nar::Socket *skt, nar::Global *globals){
	//std::map <std::string, nar::Socket*> peerList;
	std::map <std::string, std::pair< std::pair< std::string, unsigned long > ,std::pair < nar::Socket *, int > > > peerList; // token-> <chunkid,chunksize>,<socket,port>	
	setServerConnection(*skt,globals);
	
	//std::string serverReq;
	nlohmann::json serverReq;
	while(1){
		try {
			std::string message = nar::get_message( *skt);
			std::cout << message << std::endl;
			serverReq = nlohmann::json::parse(message);std::cout << serverReq.dump() << std::endl;
			std::string action = serverReq["header"]["action"];
			std::cout << "!!!!!!!!!!" << std::endl;

			if(action == "wait_chunk_request"){
				std::pair< std::string, unsigned long > p1 = std::make_pair(serverReq["payload"]["chunk-id"].get<std::string>(),serverReq["payload"]["chunk-size"].get<unsigned long>());std::cout << "Begin" << std::endl;
				std::pair< nar::Socket *, int > p2 = wait_chunk();
				peerList[serverReq["payload"]["token"]] = std::make_pair(p1,p2);
				
			}
		}
		catch ( nar::Exception ex){
			if(std::string(ex.what()) == std::string("Connection Lost")){
				std::cout << "Catch" << std::endl;
				skt->close();
				setServerConnection(*skt,globals);
					std::cout << "Reset" << std::endl;
				continue;
			}
		}
		serverReq.clear();
		std::cout << "HEREEEEE" << serverReq.dump() << std::endl; 
	}
	
	return;
}

int main() {
    nar::IPCServer cli_server(std::string("/tmp/nar_ipc"));
    cli_server.initialize();
    nar::Global* globals = new nar::Global();
    globals->set_username(std::string("nar_admin"));
	/*
	// 				Create KeepAlive Task
	nar::Socket serverSck;     
	std::thread keepalvThread(keepAlive,&serverSck,globals);
	keepalvThread.detach();
	// 				Create KeepAlive Task     
	*/
    while(true) {
        int sockfd = cli_server.acceptConnection();
        std::thread ipc_thread(handle_cli_ipc, sockfd, globals);
        ipc_thread.detach();
    }
}
