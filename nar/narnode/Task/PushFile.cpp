#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
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

}

void nar::task::PushFile::recvJson(nlohmann::json &req, nar::Socket *serverSck ){

	char *buffer = new char[1024];
	nar::Socket sk = *serverSck;
	std::string json_string = nar::get_message( sk );	
	std::cout << json_string << std::endl;
	req.parse(json_string);

}

void nar::task::PushFile::distributeFile(nlohmann::json &msg, nar::Socket *serverSck){
	
	std::cout << "HERE?4" << std::endl;
	auto chunkSize = msg["payload"]["chunk-size"].get<int>();
		std::cout << "HERE?5" << std::endl;
	std::string fileId = msg["file-id"];
	std::cout << "File id: " <<fileId << std::endl; 
	/*for(nlohmann::json::iterator it = j.begin(); it != j.end(); ++it){
		
	}*/
	 
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

    std::cout << "here" << std::endl;

    if(ITask::handshake(*serverSck, globals)) {
        std::cout << "handshake done" << std::endl;
    }

	sendJson(msg,serverSck); 							// Send Push File REQ
	
	nlohmann::json response;

	recvJson(response, serverSck);
	
	distributeFile(response, serverSck);
}


