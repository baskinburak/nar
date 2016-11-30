#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/utility.h>
#include <iostream>


static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };

//nar::ClientSocket *establishConnection( std::string peerIp, int peerPort);
nar::Socket *nar::task::PushFile::establishServerConnection(nar::Global* globals){
	nar::Socket *serverSck = new nar::Socket();
	serverSck->create();
	serverSck->connect(globals->get_narServerIp(),globals->get_narServerPort());
	return serverSck;
}

nar::Socket *nar::task::PushFile::establishPeerConnection(std::string peerIp, int peerPort){
	nar::Socket *peerSck = new nar::Socket();
	peerSck->create();
	if(peerSck->connect(peerIp ,peerPort))
	return peerSck;
	return NULL;
}

void nar::task::PushFile::getJsonHeader(rapidjson::Document &header ){

    header.SetObject();
    rapidjson::Document::AllocatorType& allocator = header.GetAllocator();

    header.AddMember("channel", "ps", allocator);
    header.AddMember("action", "file_push_request", allocator);
	return;
}
void nar::task::PushFile::getJsonPayload(rapidjson::Document &payload, std::string fileName, unsigned long fileSize,std::string dir ){
	payload.SetObject();
    rapidjson::Document::AllocatorType& allocator = payload.GetAllocator();
	rapidjson::Value fName;
	fName.SetString(fileName.c_str(), fileName.size(),allocator);
	rapidjson::Value fSize(fileSize);

	rapidjson::Value dirName;
	dirName.SetString(dir.c_str(), dir.size(),allocator);

    payload.AddMember("file-size", fSize, allocator);
    payload.AddMember("file-name", fName, allocator);
    payload.AddMember("directory", dirName, allocator);
	//payload.AddMember("file-size",)

}

void nar::task::PushFile::sendJson(rapidjson::Document &msg,nar::Socket *serverSck ){

	rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    msg.Accept(writer);
    std::string stringify(buffer.GetString());
	stringify.insert(0,std::to_string(stringify.size())+" ");

	char * writable = new char[stringify.size() + 1];
	std::copy(stringify.begin(), stringify.end(), writable);
	writable[stringify.size()] = '\0';
	std::cout << writable << std::endl;
	serverSck->send(writable, stringify.size());
    delete [] writable;

}

bool nar::task::PushFile::sendJson(nlohmann::json &req,nar::Socket *serverSck ){

    std::string stringify(req.dump() );
	stringify.insert(0,std::to_string(stringify.size())+" ");

	char * writable = new char[stringify.size() + 1];
	std::copy(stringify.begin(), stringify.end(), writable);
	writable[stringify.size()] = '\0';
	std::cout << writable << std::endl;
	return serverSck->send(writable, stringify.size());					// Free writable
}


void nar::task::PushFile::recvJson(nlohmann::json &req, nar::Socket *serverSck ){

	std::string json_string = nar::get_message( *serverSck );
	std::cout << json_string << std::endl;
	req = nlohmann::json::parse(json_string);
}

void nar::task::PushFile::getPeerInfo(std::string peerId, nar::Socket *serverSck){
	nlohmann::json req;
	req["header"]["channel"] = "ps";
	req["header"]["action"] = "peer_connection_request";
	req["payload"]["peer-id"] = peerId;
	sendJson(req, serverSck);
}

nar::Socket *nar::task::PushFile::sendTokenToPeer(nlohmann::json::iterator &it, nar::Socket *serverSck){

	nar::Socket *peerSck;
	std::string peerId = (*it)["payload"]["peer-id"].get<std::string>();
	getPeerInfo(peerId, serverSck);
	nlohmann::json resp;
	recvJson(resp,serverSck);

	int peerPort = resp["payload"]["peer-port"];
	std::string peerIp = resp["payload"]["peer-ip"];

	peerSck = establishPeerConnection(peerIp,peerPort);


	while(peerSck == NULL){
		nlohmann::json req;
		req["header"]["channel"] = "ps";
		req["header"]["action"] = "new_peer_request_for_chunk";
		req["payload"]["peer-id"] = peerId;
		req["payload"]["chunk-id"] = (*it)["chunk-id"].get<std::string>();
		req["payload"]["chunk-size"] =(*it)["chunk-size"].get<int>();
		sendJson(req,serverSck);
		req.clear();
		nlohmann::json resp;
		recvJson(resp,serverSck);
		peerId = (*it)["peer_id"].get<std::string>();
		getPeerInfo(peerId,serverSck);
		resp.clear();
		recvJson(resp,serverSck);
		peerPort = resp["payload"]["peer-port"];
		peerIp = resp["payload"]["peer-ip"];
		peerSck = establishPeerConnection(peerIp,peerPort);
	}
		nlohmann::json req;
		resp.clear();
		req["header"]["channel"] = "pp";
		req["header"]["action"] = "peer_authentication_request";
		req["payload"]["token"] = (*it)["token"].get<std::string>();


		recvJson(resp,serverSck);							// WHAT TO GET HERE

		return peerSck;

}

void nar::task::PushFile::pushFileToPeer(nlohmann::json::iterator &it, nar::Socket *peerSck, nar::FileKeeper &file, size_t &fOffset){
	unsigned long chunkSize = (*it)["chunk-size"].get<int>();    	// MAKE IT UNSIGNED LONG

	char *chunk = new char[chunkSize];
	unsigned long len = file.getBytes(fOffset,  chunkSize,chunk);

	peerSck->send(chunk, len);
	fOffset += len;
}

void nar::task::PushFile::distributeFile(nlohmann::json &msg, nar::Socket *serverSck, nar::FileKeeper &file){

	int chunkSize = msg["payload"]["chunk-size"].get<int>();
	std::string fileId = msg["payload"]["file-id"];
	std::cout << "File id: " <<fileId << std::endl;
	size_t fOffset = 0;
	for(nlohmann::json::iterator it = msg["payload"]["peer-list"].begin(); it != msg["payload"]["peer-list"].end(); ++it){
		/*std::cout << "HELP5" << std::endl;
		std::string peerId= (*it)["peer_id"];
		std::cout << "HELP6" << std::endl;
		std::string token = (*it)["token"];
				std::cout << "HELP7" << std::endl;
		getPeerInfo(peerId, serverSck);

		nlohmann::json resp;
		recvJson(resp,serverSck);

		int peerPort = resp["payload"]["peer-port"];
		std::string peerIp = resp["payload"]["peer-ip"];

		nar::Socket *peerSck = establishPeerConnection(peerIp,port);
		resp.clear();
		resp["payload"][]*/

		nar::Socket *peerSck = sendTokenToPeer(it,serverSck);
		pushFileToPeer(it,peerSck, file , fOffset);
	}
}


void nar::task::PushFile::run(int unx_sockfd, nar::Global* globals) {

	nar::FileKeeper file(file_path);
	unsigned long file_size = file.getFileSize();

	rapidjson::Document msg;
	rapidjson::Document::AllocatorType& allocator = msg.GetAllocator();
	msg.SetObject();

	rapidjson::Document header(&msg.GetAllocator());		// Create Header
	getJsonHeader(header);

	rapidjson::Document payload(&msg.GetAllocator());		// Create Payload
	getJsonPayload(payload,file.getFileName(),file.getFileSize(),globals->get_curdir());

	msg.AddMember("header:", header, msg.GetAllocator());
	msg.AddMember("payload:", payload, msg.GetAllocator());

	nar::Socket *serverSck = establishServerConnection(globals); // Connect to Server



    /*if(ITask::handshake(*serverSck, globals)) {
        std::cout << "handshake done" << std::endl;
    }*/

	sendJson(msg,serverSck); 							// Send Push File REQ

	nlohmann::json response;

	recvJson(response, serverSck);

	distributeFile(response, serverSck, file);
}
