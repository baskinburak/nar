#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <iostream>
#include <string>
#include <nar/lib/cptl.hpp>
#include <nar/lib/Socket/USocket.h>
#include <nar/narnode/File/File.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>

using std::string;
using std::cout;
using std::endl;

void nar::ActiveTask::Push::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPush::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    std::string file_aes;
    try {
        file_aes = nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch(nar::Exception::ExcpBase& excp) {
        throw;
    }

    string aes = file_aes;

    string file_path = req->get_file_path();

    nar::File original(file_path, "r", false);
    nar::File* compressed = original.compress();
    nar::File* encrypted = compressed->encrypt(aes);

    unsigned long file_size = encrypted->size();
    string pushdir = this->_vars->get_current_directory();


    string file_name;
    for(int i=file_path.size()-1; i>=0; i--) {
        if(file_path[i] == '/') break;
        file_name.push_back(file_path[i]);
    }
    std::reverse(file_name.begin(), file_name.end());

    std::cout << "push 1<<" << std::endl;

    nar::MessageTypes::FilePush::Request push_req(file_name, pushdir, file_size);
    nar::MessageTypes::FilePush::Response push_resp;
    push_req.send_mess(server_sck, push_resp);
    
    std::cout << "push 2<<" << std::endl;
    std::vector<nar::MessageTypes::FilePush::Response::PeerListElement> elements = push_resp.get_elements();


    std::cout << "push 3<<" << std::endl;
    unsigned long start = 0;
    for(int i=0; i<elements.size(); i++) {
        boost::asio::io_service& ioserv = this->_globals->get_ioserv();
        nar::USocket* usck = new nar::USocket(ioserv, this->_globals->get_server_ip(), push_resp.get_randezvous_port(), elements[i].stream_id);
        usck->connect();
        std::cout << elements[i].chunk_size << std::endl;
        usck->send(*encrypted, start, elements[i].chunk_size);
        start += elements[i].chunk_size;
    }

    std::cout << "push 4<<" << std::endl;

    delete compressed;
    delete encrypted;
}
