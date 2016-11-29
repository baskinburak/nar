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
    std::cout << "a" << std::endl;
	serverSck -> connect(peer_ip, peer_port); //acılamazsa hata don!
    std::cout << "b" << std::endl;
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
    std::cout << "gotham" << std::endl;
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

void nar::task::PullFile::comeTogether(nlohmann::json j_resp, nar::Socket *serverSck)
{
    //peer list in our hand now!
    std::cout << "asd" << std::endl;
    int chunk_size = j_resp["payload"]["chunk-size"].get<int>();
    std::cout << chunk_size << std::endl;
    std::cout << j_resp["payload"]["peer-list"].size() << std::endl;

    for(int i = 0 ; i < j_resp["payload"]["peer-list"].size() ; i++) {
        std::cout << j_resp["payload"]["peer-list"].at(i) << std::endl;
        //{"chunk-id":1,"peer-id":341}
        nlohmann::json j_for_peer;
        j_for_peer["header"]["channel"] = "ps";
        j_for_peer["header"]["action"] = "peer_connection_request";
        j_for_peer["payload"]["peer-id"] = (j_resp["payload"]["peer-list"].at(i))["peer-id"];
        std::cout << (j_resp["payload"]["peer-list"].at(i))["peer-id"] << std::endl;

        sendRequestJson(j_for_peer, serverSck);

        nlohmann::json j_get_peerInfo;

        getResultJson(j_get_peerInfo, serverSck);
        //we have peer ip and peer port NOW!
        std::cout << j_get_peerInfo << std::endl;

        std::string peer_ip = j_get_peerInfo["payload"]["peer-ip"];
        int peer_port = j_get_peerInfo["payload"]["peer-port"];
        std::cout << "peer IP: " << peer_ip << std::endl;
        std::cout << "peer Port: " << peer_port << std::endl;
        //NOW,connect to peer!
        std::cout << "connect to peer with peer-ip and peer-port" << std::endl;
        nar::Socket *soc_to_peer = connectToPeer(peer_ip,  peer_port);
        std::cout << "connected!" << std::endl;
        //chunk size,chunk id,token
        nlohmann::json j_for_mypeer;
        constructJsonforPeer(j_for_mypeer, );
        //after token matched, pull files!
    }
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
