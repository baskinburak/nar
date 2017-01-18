#include <nar/narnode/Task/PullFile.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <nar/narnode/FileKeeper/FileDecryptor.h>
#include <nar/narnode/utility.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fstream>
void nar::task::PullFile::initialize()
{
    nar::FileKeeper file(file_name);
    std::cout << "initializer done!" << std::endl;
    return;
}

void nar::task::PullFile::constructRequestJson(nlohmann::json &j, std::string dirname)
{
    j["header"] = { {"channel", "ps"}, {"action", "file_pull_request"} };

    j["payload"] = { {"file_name", file_name}, {"directory", dirname}};
    std::cout << "json constructed!" << std::endl;
    return;
}

void nar::task::PullFile::constructJsonforPeer(nlohmann::json &j, std::string dirname)
{
    j["header"] = { {"channel", "ps"}, {"action", "file_pull_request"} };

    j["payload"] = { {"file_name", file_name}, {"directory", dirname}};
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

void nar::task::PullFile::getResultJsonFake(nlohmann::json &j_resp, nar::Socket *serverSck)
{
    return;
}

void nar::task::PullFile::getPeerInfo(std::string peerId, nar::Socket *serverSck, std::string _token){
    //std::cout << "gotham artik" << std::endl;
    nlohmann::json req;
	req["header"]["channel"] = "ps";
	req["header"]["action"] = "peer_connection_request_pull";
	req["payload"]["peer-id"] = peerId;
	req["payload"]["token"] = _token;
    //std::cout << "hadi canim" << std::endl;
	sendRequestJson(req, serverSck);
    std::cout << "hadi canim1" << std::endl;
    return;
}

nar::Socket* nar::task::PullFile::sendTokenToPeer(nlohmann::json::iterator &it, nar::Socket *serverSck,unsigned long chunkSize)
{
    //std::cout << "gotham starts0" << std::endl;
	nar::Socket *peerSck;
	std::string peerId = (*it)["peer_id"].get<std::string>();
    //std::cout << "asd" << std::endl;
    getPeerInfo(peerId, serverSck,(*it)["token"].get<std::string>());
    //std::cout << "gotham starts1" << std::endl;
    nlohmann::json resp;
	getResultJson(resp,serverSck);
	int peerPort = resp["payload"]["peer-port"];
	std::string peerIp = resp["payload"]["peer-ip"];
    std::cout << peerIp << std::endl;
    std::cout << peerPort << std::endl;std::cout << "gotham startsXXX" << std::endl;
	peerSck = connectToPeer(peerIp,peerPort);
    std::cout << "gotham starts2" << std::endl;

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
		getPeerInfo(peerId,serverSck,(*it)["token"].get<std::string>());

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
	std::cout<<"abov"<<std::endl;
	nlohmann::json req;
	resp.clear();
	req["header"]["channel"] = "pp";
	req["header"]["action"] = "chunk_receive_request";
	req["payload"]["token"] = (*it)["token"].get<std::string>();
    req["payload"]["chunk-id"] = (*it)["chunk_id"].get<std::string>();
    req["payload"]["chunk-size"] = chunkSize;
    //std::cout << "token: " << req["payload"]["token"] << std::endl;
	std::cout << "bla" << std::endl;
    sendRequestJson(req, peerSck);
    std::cout << get_message(*peerSck) << std::endl;

    //  BURDA ONAYLAMIS GIBI JSON ATTGINI DUSUN!
    std::cout << "get json from peer which is fake!" << std::endl;
    //getResultJsonFake(resp, peerSck);
    //std::cout << "peerSck: " << *peerSck << std::endl;
	return peerSck;
}

void nar::task::PullFile::pullFileFromPeer(nlohmann::json::iterator &it, nar::Socket *peerSck, unsigned long chunkSize, std::string aes,int file)
{/*
    std::string fileName = (*it)["chunk-id"].get<std::string>();
    nar::FileKeeper file(fileName);
    char buffer[1024] = " ";
    while(peerSck->recv(buffer,chunkSize) > 0);
    int index=0;
    std::cout << buffer[0] << std::endl;
    //while() index++;
    //file.writeToFile(file.getFd(), index, (const char *)buffer);
    std::cout << "sanirim bitti" << std::endl;
    return;*/
   																			// std::string path("/tmp/maq");
    //int fd = nar::FileKeeper::openFdWrtonly( path.c_str());
    //nar::FileKeeper f( path );
    //if(! nar::readSckWriteFile(fd,*peerSck,chunkSize)) return;

    //std::string path("/home/utku/NarStorage/");
    //path = path + chunkId;
    //std::cout << path << std::endl;

    																	//	std::cout << "chuSzi " << chunkSize << std::endl;

    //std::cout << nar::get_string_sckt(peerSck->getSckDescriptor(),chunkSize) << std::endl;
																	//decyrpt it
																	//std::string destination = cur_dir + std::string("/") + file_name;
																	//std::cout<<destination<<std::endl;


																	//nar::FileDecryptor fdec(nar::get_string_sckt(peerSck->getSckDescriptor(),chunkSize), aes);
																	//nar::FileKeeper* f = fdec.decrypt(destination);
	std::cout << "CS: " << chunkSize << std::endl;
	std::string cnk = nar::get_string_sckt(peerSck->getSckDescriptor(),chunkSize);

		std::cout << "HERE  " << cnk.substr(0,5) << std::endl;

	nar::FileKeeper::writeToFile(file, chunkSize, cnk.c_str());	
	std::cout << "ENDK" << std::endl;
    return;

}

void nar::task::PullFile::comeTogether(nlohmann::json &j_resp, nar::Socket *serverSck,std::string aes)
{
   // unsigned long chunkSize = j_resp["payload"]["chunk-size"].get<int>();
	std::string destination = cur_dir + std::string("/") + std::string("tmpFile");//file_name;
    std::cout << j_resp.dump() << std::endl;
	int holder = nar::FileKeeper::openFdWrtonlyAppend(destination.c_str());
  	
    for(nlohmann::json::iterator it = j_resp["payload"]["peer-list"].begin(); it != j_resp["payload"]["peer-list"].end(); ++it) {
        //std::cout << "are you here,in come0?" << std::endl;
        std::cout << "(*it)" << std::endl;
        nar::Socket *peerSck = sendTokenToPeer(it, serverSck,(*it)["chunk_size"]);
        std::cout << "are you here,in come1?" << std::endl;
        pullFileFromPeer(it, peerSck,(*it)["chunk_size"],aes,holder); // 100 -> chunksize
        std::cout << "are you here,in come2?" << std::endl;
    }
	close(holder);

	nar::FileKeeper keep(destination);
	int size = keep.getFileSize();
	std::cout<<"SIIIIIIIZZZZZZZZZZZZE "<<size<<std::endl;
    char* buff = (char*) malloc(sizeof(char) * size);
    keep.getBytes(0, size, buff);
std::cout<<"SIIIIIIIZZZZZZZZZZZZE2 "<<size<<std::endl;
	std::string var(buff,size);
	//std::cout << var << std::endl << std::endl;
	std::cout << "size aes**********:  " << var.size() << " " << aes << std::endl;
	nar::FileDecryptor fdec(var, aes);
	delete[] buff;
	fdec.decrypt( cur_dir + std::string("/") + file_name );
	std::remove(destination.c_str());
    return;
}

std::string nar::task::PullFile::getAes(nar::Socket *serverSck)
{
    std::string res;
    nlohmann::json j_for_aes;
    j_for_aes["header"]["channel"] = "ps";
    j_for_aes["header"]["action"] = "get_aes_request";

    sendRequestJson(j_for_aes, serverSck);

    nlohmann::json j_resp;
    getResultJson(j_resp, serverSck);

    std::string result = j_resp.dump();
    res = j_resp["payload"]["aes"];
    return res;
}

void nar::task::PullFile::run(int unx_sockfd, nar::Global* globals) {
    nlohmann::json j_sent,j_resp;
    //usleep(3000000);

    std::cout << unx_sockfd << std::endl;
    initialize();

    constructRequestJson(j_sent, globals -> get_curdir());
    std::cout << "Cur directory: " << globals -> get_curdir() << std::endl;

    nar::Socket *server_sck = connectToServer(globals);

    std::cout << "connection to server, done!" << std::endl;

    if(ITask::handshake(*server_sck, globals)) {
        std::cout << "handshake done" << std::endl;
    }

    std::cout << "run pull file" << std::endl;

    std::string aes = getAes(server_sck);

    std::cout << "aes key " << aes << std::endl;

    sendRequestJson(j_sent, server_sck);

    getResultJson(j_resp, server_sck); // and parse
    std::cout << j_resp << std::endl;
    std::cout << "finito" << std::endl;
    comeTogether(j_resp,server_sck,aes);  //pull all files from peers

    std::string finMsg("END");
    nar::send_ipc_message(unx_sockfd,finMsg);
    return;
}
