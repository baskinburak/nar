#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/utility.h>
#include <iostream>
#include <nar/narnode/FileKeeper/FileCompressor.h>

static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };

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

void nar::task::PushFile::getPeerInfo(std::string peerId,std::string token, nar::Socket *serverSck){
	nlohmann::json req;
	req["header"]["channel"] = "ps";
	req["header"]["action"] = "peer_connection_request";
	req["payload"]["peer-id"] = peerId;
	req["payload"]["token"] = token;
	sendJson(req, serverSck);
}

nar::Socket *nar::task::PushFile::sendTokenToPeer(nlohmann::json::iterator &it, nar::Socket *serverSck , int chunkSize){

	nar::Socket *peerSck; std::cout << "Not here eitherX" << std::endl;
	std::string peerId = (*it)["peer-id"].get<std::string>(); std::cout << "Not here eitherYYY      " << (*it).dump() << std::endl;
	getPeerInfo(peerId, (*it)["token"],serverSck);	std::cout << "Not here eitherZZ" << std::endl;
	nlohmann::json resp;
	//recvJson(resp,serverSck);	std::cout << "Not here eitherKKKK" << std::endl;
	recvJson(resp,serverSck);	std::cout << "Not here eitherKKKK" << std::endl;
	std::cout << "Not here eitherLLL" << resp.dump()<< std::endl;
	int peerPort = resp["payload"]["peer-port"]; std::cout << "Not here eitherDDD" << std::endl;
	std::string peerIp = resp["payload"]["peer-ip"]; std::cout << "Not here eitherOOO" << std::endl;

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
		getPeerInfo(peerId, (*it)["token"],serverSck);
		resp.clear();
		recvJson(resp,serverSck);
		peerPort = resp["payload"]["peer-port"];
		peerIp = resp["payload"]["peer-ip"];
		peerSck = establishPeerConnection(peerIp,peerPort);
	}
		nlohmann::json req;
		resp.clear();

std::cout << "Not here eitherTTTT" << std::endl;

		req["header"]["channel"] = "pp";std::cout << "Not here eitherzsdsfvsdf" << std::endl;
		req["header"]["action"] = "chunk_send_request"; std::cout << "Not here eitherzsdsfvsdf" << std::endl;
		req["payload"]["token"] = (*it)["token"].get<std::string>(); std::cout << "Not here eitherzsdsfvsdf" << std::endl;
		req["payload"]["chunk-id"] = (*it)["chunk-id"].get<std::string>(); std::cout << "Not here eitherzsdsfvsdf" << std::endl;
		req["payload"]["chunk-size"] = chunkSize; std::cout << "Not here eitherzsdsfvsdf" << std::endl;
		std::cout << "Not here eitherasdasda" << std::endl;
		send_message(peerSck, req.dump());



		//recvJson(resp,serverSck);							// WHAT TO GET HERE

		return peerSck;

}

void nar::task::PushFile::pushFileToPeer(nlohmann::json::iterator &it, nar::Socket *peerSck, nar::FileCryptor &file, size_t &fOffset,int _chunkSize){
	/*unsigned long chunkSize = _chunkSize;    	// MAKE IT UNSIGNED LONG

	char *chunk = new char[chunkSize];
	unsigned long len = file.getBytes(fOffset,  chunkSize,chunk);

	peerSck->send(chunk, len);
	fOffset += len;*/
	std::cout << "MATTERSSS!!!!! *** CS OFF "<<_chunkSize << " " << fOffset << std::endl;
	nar::readFileWriteSck( file, *peerSck, _chunkSize,fOffset);


}

void nar::task::PushFile::distributeFile(nlohmann::json &msg, nar::Socket *serverSck, nar::FileCryptor &file, nar::Global* globals){
	std::cout << "Not here eitherEHE" << std::endl;
	std::cout << msg.dump() << std::endl;
	//int chunkSize = msg["payload"]["chunk-size"].get<int>();std::cout << "Not here eitherAHA" << std::endl;
	unsigned short rand_port = msg["payload"]["rand-port"];
	size_t fOffset = 0;
	for(nlohmann::json::iterator it = msg["payload"]["peer-list"].begin(); it != msg["payload"]["peer-list"].end(); ++it){

        unsigned int stream_id = (*it)["stream-id"];
        unsigned long chunk_size = (*it)["chunk-size"];
        std::string machine_id = (*it)["peer-id"];
        std::string chunk_id = (*it)["chunk-id"];

        nar::USocket peer_sock(stream_id);
        peer_sock.make_randevous(globals->get_narServerIp(), rand_port);

        nar::readFileWriteSck(file, peer_sock, chunk_size, fOffset);
		fOffset += chunk_size;
		//peerSck->close();
	}
}

std::string nar::task::PushFile::getAesKey(nar::Socket *serverSck) {
	nlohmann::json req;
	req["header"]["action"] = std::string("get_aes_request");
	req["header"]["channel"] = "ps";
	send_message(serverSck, req.dump());
	nlohmann::json resp;
	recvJson(resp, serverSck);
	std::cout << resp.dump() << std::endl;
	return resp["payload"]["aes"];
}


void nar::task::PushFile::run(int unx_sockfd, nar::Global* globals) {

	nar::Socket *serverSck = establishServerConnection(globals); // Connect to Server


	rapidjson::Document msg;
	rapidjson::Document::AllocatorType& allocator = msg.GetAllocator();
	msg.SetObject();




    if(ITask::handshake(*serverSck, globals)) {
        std::cout << "handshake done" << std::endl;
    }

	std::string aes = getAesKey(serverSck);

	nar::FileCompressor fcomp;
    std::string comPath(std::getenv("HOME"));
    comPath += std::string("/nar/compFile");
    fcomp.compress_one_file(file_path,comPath);

  std::string filename = file_path;
  std::string res;
  while(filename[filename.size()-1] != '/') {
      res.push_back(filename[filename.size()-1]);
     filename.pop_back();
  }
  std::reverse(res.begin(), res.end());

    nar::FileKeeper f(comPath);
	nar::FileCryptor file(&f, aes);
	unsigned long file_size = file.getFileSize();

	rapidjson::Document header(&msg.GetAllocator());		// Create Header
	getJsonHeader(header);

	rapidjson::Document payload(&msg.GetAllocator());		// Create Payload
	getJsonPayload(payload,res,file.getFileSize(),globals->get_curdir());

	msg.AddMember("header", header, msg.GetAllocator());
	msg.AddMember("payload", payload, msg.GetAllocator());


	sendJson(msg,serverSck); 							// Send Push File REQ

	nlohmann::json response;

	recvJson(response, serverSck);

	distributeFile(response, serverSck, file, globals);
  std::remove(comPath.c_str());
	std::string endMsg("END");
	nar::send_ipc_message(unx_sockfd, endMsg);
}
