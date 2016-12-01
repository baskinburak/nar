#include <nar/narnode/Task/PullFile.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <nar/narnode/utility.h>
#include <iostream>

void nar::task::PullFile::initialize()
{
    nar::FileKeeper file(file_name);
	file_size = file.getFileSize();
    std::cout << "initializer done!" << std::endl;
    return;
}

void nar::task::PullFile::constructRequestJson(nlohmann::json &j, std::string dirname)
{
    j["header"] = { {"channel", "ps"}, {"action", "file_pull_request"} };

    j["payload"] = { {"file_size", file_size}, {"file_name", file_name}, {"directory", dirname}};
    std::cout << "json constructed!" << std::endl;
    return;
}

void nar::task::PullFile::constructJsonforPeer(nlohmann::json &j, std::string dirname)
{
    j["header"] = { {"channel", "ps"}, {"action", "file_pull_request"} };

    j["payload"] = { {"file_size", file_size}, {"file_name", file_name}, {"directory", dirname}};
    std::cout << "json constructed!" << std::endl;
    return;
}

void nar::task::PullFile::constructJsonforNewPeer(nlohmann::json::iterator &it, nlohmann::json &j, std::string peer_id)
{
    j["header"]["channel"] = "ps";
    j["header"]["action"] = "new_peer_request_for_chunk";
    j["payload"]["peer-id"] = peer_id;
    j["payload"]["chunk-id"] = (*it)["chunk-id"].get<std::string>();
    j["payload"]["chunk-size"] = (*it)["chunk-size"].get<int>();
    return;
}

nar::Socket* nar::task::PullFile::connectToServer(nar::Global* globals)
{
    nar::Socket *serverSck = new nar::Socket();
	if(serverSck->create() == false)
    {
        std::cout << "aha burda patladin gotham create" << std::endl;
    }
	if(serverSck->connect(globals -> get_narServerIp(), globals -> get_narServerPort()) == false)
    {
        std::cout << "aha burda patladin gotham connect" << std::endl;
        std::cout << globals -> get_narServerIp() << std::endl;
        std::cout << globals -> get_narServerPort() << std::endl;
    }
    return serverSck;
}

nar::Socket* nar::task::PullFile::connectToPeer(std::string peer_ip, int peer_port)
{
    nar::Socket *serverSck = new nar::Socket();
	serverSck -> create();
    //std::cout << "a" << std::endl;
	serverSck -> connect(peer_ip, peer_port); //acılamazsa hata don!
    //std::cout << "b" << std::endl;
	return serverSck;
}

void nar::task::PullFile::sendRequestJson(nlohmann::json &j, nar::Socket *server_sck)
{
    std::string json_str = j.dump();
    std::string tmp = std::to_string(json_str.length());
    tmp += " ";
    std::cout << tmp << std::endl;
    json_str.insert(0, tmp);

    char * str_to_charp = new char[json_str.length() + 1];
    strncpy(str_to_charp, json_str.c_str(), json_str.length());
    str_to_charp[json_str.length()] = '\0';

    server_sck -> send(str_to_charp ,json_str.length() + 1);
    //std::cout << "gotham" << std::endl;
    delete str_to_charp;
}

void nar::task::PullFile::getResultJson(nlohmann::json &j_resp, nar::Socket *serverSck)
{
	std::string json_to_string = nar::get_message( *serverSck );
    std::cout << "and" << std::endl;
	std::cout << json_to_string << std::endl;
	j_resp = nlohmann::json::parse(json_to_string); //string -> json
    std::cout << "or" << std::endl;
}

void nar::task::PullFile::getPeerInfo(std::string peerId, nar::Socket *serverSck){
    //std::cout << "gotham artik" << std::endl;
    nlohmann::json req;
	req["header"]["channel"] = "ps";
	req["header"]["action"] = "peer_connection_request";
	req["payload"]["peer-id"] = peerId;
    //std::cout << "hadi canim" << std::endl;
	sendRequestJson(req, serverSck);
    //std::cout << "hadi canim1" << std::endl;
    return;
}

nar::Socket* nar::task::PullFile::sendTokenToPeer(nlohmann::json::iterator &it, nar::Socket *serverSck)
{
    //std::cout << "gotham starts0" << std::endl;
	nar::Socket *peerSck;
	std::string peerId = (*it)["peer-id"].get<std::string>();
    //std::cout << "asd" << std::endl;
    getPeerInfo(peerId, serverSck);
    //std::cout << "gotham starts1" << std::endl;
    nlohmann::json resp;
	getResultJson(resp,serverSck);
	int peerPort = resp["payload"]["peer-port"];
	std::string peerIp = resp["payload"]["peer-ip"];
    std::cout << peerIp << std::endl;
    std::cout << peerPort << std::endl;
	peerSck = connectToPeer(peerIp,peerPort);
    //std::cout << "gotham starts2" << std::endl;
    while(peerSck == NULL){
		nlohmann::json req;
        constructJsonforNewPeer(it, req, peerId);
        //std::cout << "gotham starts3" << std::endl;
		sendRequestJson(req,serverSck);

		req.clear();
        //std::cout << "gotham starts4" << std::endl;

		nlohmann::json resp;
		getResultJson(resp,serverSck);

		peerId = (*it)["peer_id"].get<std::string>();
        std::cout << "peer id: " << peerId << std::endl;
		getPeerInfo(peerId,serverSck);

        //std::cout << "gotham starts5" << std::endl;

        resp.clear();
		getResultJson(resp,serverSck);
        //std::cout << "gotham starts6" << std::endl;
        peerPort = resp["payload"]["peer-port"];
        std::cout << "peer port:" << peerPort << std::endl;
		peerIp = resp["payload"]["peer-ip"];
		peerSck = connectToPeer(peerIp,peerPort);

        //std::cout << "gotham starts7" << std::endl;
	}
	nlohmann::json req;
	resp.clear();
	req["header"]["channel"] = "pp";
	req["header"]["action"] = "peer_authentication_request";
	req["payload"]["token"] = (*it)["token"].get<std::string>();
    std::cout << "token: " << req["payload"]["token"] << std::endl;
    getResultJson(resp, serverSck);
	return peerSck;
}

void nar::task::PullFile::pullFileFromPeer(nlohmann::json::iterator &it, nar::Socket *peerSck, int chunk_id)
{
    //std::cout << "are you here,in push" << std::endl;
    std::string fileName = std::to_string(chunk_id);
    nar::FileKeeper file(fileName);
    //std::cout << "are you here,in push2" << std::endl;
    int byteReceived = 0;
    char *buffer = new char[1024];
    //std::cout << "are you here,in push3" << std::endl;
    size_t lastIndex = 0;
    while(byteReceived = file.getBytes(lastIndex, 1024, buffer))
    {
        //std::cout << "bytes:" << byteReceived << std::endl;
        //file.writeToFile(, 512, buffer);
        lastIndex += byteReceived;
    }
    //std::cout << "are you here,in push4" << std::endl;
    delete buffer;

}

void nar::task::PullFile::comeTogether(nlohmann::json j_resp, nar::Socket *serverSck)
{
    int chunkSize = j_resp["payload"]["chunk-size"].get<int>();
    std::cout << chunkSize << std::endl;
    for(nlohmann::json::iterator it = j_resp["payload"]["peer-list"].begin(); it != j_resp["payload"]["peer-list"].end(); ++it) {
        //std::cout << "are you here,in come0?" << std::endl;
        std::cout << (*it) << std::endl;
        nar::Socket *peerSck = sendTokenToPeer(it, serverSck);
        //std::cout << "are you here,in come1?" << std::endl;
        pullFileFromPeer(it, peerSck, j_resp["payload"]["chunk-id"]);
        //std::cout << "are you here,in come2?" << std::endl;
    }
    return;
}

void nar::task::PullFile::run(int unx_sockfd, nar::Global* globals) {
    nlohmann::json j_sent,j_resp;

    initialize();

    constructRequestJson(j_sent, globals -> get_curdir());

    nar::Socket *server_sck = connectToServer(globals);

    std::cout << "connection to server, done!" << std::endl;

    /*
    if(ITask::handshake(*server_sck, globals)) {
        std::cout << "handshake done" << std::endl;
    }
    */

    std::cout << "run pull file" << std::endl;

    sendRequestJson(j_sent, server_sck);

    std::cout << "here" << std::endl;

    getResultJson(j_resp, server_sck); // and parse
    std::cout << j_resp << std::endl;
    std::cout << "finito" << std::endl;
    comeTogether(j_resp,server_sck);  //pull all files from peers

}
