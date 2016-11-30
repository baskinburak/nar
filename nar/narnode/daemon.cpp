#include <nar/narnode/IPC/ipcserver.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
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
#include <algorithm>

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
        nar::task::LS task(doc["dir"].GetString());
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

	tmp->create();

	if(!tmp->bind(0)) std::cout<< "Error on bind " << std::endl;

	if(!tmp->listen()) std::cout << "Listen Error" << std::endl;

	
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(tmp->getSckDescriptor(), (struct sockaddr *)&sin, &len) == -1)
    	std::cout << "getsockname failed" << std::endl;
	
    int port = ntohs(sin.sin_port);
	std::cout << "Port: " << port << std::endl;

	return std::make_pair(tmp,port);
}


int wait_on_sock(int sock, long timeout, int r, int w)
{
    struct timeval tv = {0,0};
    fd_set fdset;
    fd_set *rfds, *wfds;
    int n, so_error;
    unsigned so_len;

    FD_ZERO (&fdset);
    FD_SET  (sock, &fdset);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    /*TRACES ("wait in progress tv={%ld,%ld} ...\n",
            tv.tv_sec, tv.tv_usec); */

    if (r) rfds = &fdset; else rfds = NULL;
    if (w) wfds = &fdset; else wfds = NULL;

    TEMP_FAILURE_RETRY (n = select (sock+1, rfds, wfds, NULL, &tv));
    switch (n) {
    case 0:
        std::cout << "wait timed out\n";
        return -errno;
    case -1:
        std::cout << "error during wait\n";
        return -errno;
    default:
        // select tell us that sock is ready, test it
        so_len = sizeof(so_error);
        so_error = 0;
        getsockopt (sock, SOL_SOCKET, SO_ERROR, &so_error, &so_len);
        if (so_error == 0)
            return 0;
        errno = so_error;
        std::cout << "wait failed\n";
        return -errno;
    }
}

void getChunkFromPeer(nar::Socket* skt, std::string chunkId, unsigned long chunkSize, std::string token){
	struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 100;
    tv.tv_usec = 000000;


	int sckDescriptor = skt->getSckDescriptor();

	std::cout << "IMA THREAD" << std::endl;

	int res =	wait_on_sock( sckDescriptor, 200 , 1, 0);
	std::cout << "It got out boi with " << res  << std::endl;

	nar::Socket peerSkt; 
	if(!skt->accept(peerSkt)) std::cout << "Error in accept" << std::endl;
	
	std::string msg = nar::get_message(peerSkt);
	nlohmann::json peerReq;
	peerReq = nlohmann::json::parse(msg);
	std::cout << "\n";
	std::cout << msg << std::endl;
	if( (peerReq["payload"]["token"].get<std::string>() != token )||( peerReq["payload"]["chunk-id"].get<std::string>() != chunkId) || (peerReq["payload"]["chunk-size"].get<unsigned long>() != chunkSize)  )
	{
		peerSkt.close();
		skt->close();
		delete skt;
		return;
	}
	std::string path("/home/utku/NarStorage/");	
	path = path + chunkId;
	std::cout << path << std::endl;
	int fd = nar::FileKeeper::openFdWrtonly( path.c_str());


	if(! nar::readSckWriteFile(fd,peerSkt,chunkSize)) return;
	
	peerSkt.close();

}

void createReqHeader( nlohmann::json &msg ,std::string action, std::string channel ){
	msg["header"]["action"] = action;
	msg["header"]["channel"] = channel;
}

void createRespHeader( nlohmann::json &msg ,std::string reply, std::string channel, std::string status ){
	msg["header"]["reply-to"] = reply;
	msg["header"]["channel"] = channel;
	msg["header"]["status"] = status;	
}


void keepAlive( nar::Socket *skt, nar::Global *globals){
	std::map <std::string, std::pair< std::pair< std::string, unsigned long > ,std::pair < nar::Socket *, int > > > peerList; // token-> <chunkid,chunksize>,<socket,port>	
	setServerConnection(*skt,globals);
	
	nlohmann::json serverReq;
	while(1){
		try {
			std::string message = nar::get_message( *skt);
			std::cout << message << std::endl;
			serverReq = nlohmann::json::parse(message);std::cout << serverReq.dump() << std::endl;
			std::string action = serverReq["header"]["action"];
			std::cout << "!!!!!!!!!!" << std::endl;

			if(action == "wait_chunk_request"){
				std::pair< std::string, unsigned long > p1 = std::make_pair(serverReq["payload"]["chunk-id"].get<std::string>(),serverReq["payload"]["chunk-size"].get<unsigned long>());
				std::cout << "Begin" << std::endl;
				std::pair< nar::Socket *, int > p2 = wait_chunk();
				peerList[serverReq["payload"]["token"]] = std::make_pair(p1,p2);
				
			}
			else if(action == "peer_port_request"){
				std::string chunkId = peerList[serverReq["payload"]["token"]].first.first;
				unsigned long chunkSize = peerList[serverReq["payload"]["token"]].first.second;
				nar::Socket* tmpSkt = peerList[serverReq["payload"]["token"]].second.first; std::cout << "Casual" << std::endl;
				int port = peerList[serverReq["payload"]["token"]].second.second;
				peerList.erase( serverReq["payload"]["token"].get<std::string>() );std::cout << "After Erase" << std::endl;
				std::thread getFile(getChunkFromPeer, tmpSkt, chunkId, chunkSize,serverReq["payload"]["token"]); // TOKEN MAYBE WRONG TOKEN
				getFile.detach();

				nlohmann::json rsp;
				createRespHeader(rsp,std::string("peer_port_request"), std::string("ps"), std::string("200"));
				rsp["payload"]["port"] = port;
				std::string rspStr(rsp.dump());

				nar::send_string_sckt ( skt->getSckDescriptor(), rspStr,rspStr.size());
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
	
	// 				Create KeepAlive Task
	nar::Socket serverSck;     
	std::thread keepalvThread(keepAlive,&serverSck,globals);
	keepalvThread.detach();
	// 				Create KeepAlive Task     

    while(true) {
        int sockfd = cli_server.acceptConnection();
        std::thread ipc_thread(handle_cli_ipc, sockfd, globals);
        ipc_thread.detach();
    }
}
