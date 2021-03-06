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

void nar::ActiveTask::Push::send_error_response(nar::Socket* ipc_socket, int statcode) { 
    nar::MessageTypes::IPCPush::Response resp(statcode);
    try {
        resp.send_message(ipc_socket);
        resp.send_message_end(ipc_socket);
    } catch(...) {
        NAR_LOG << "CLI seems down." << std::endl;
    }
    ipc_socket->close();
}

void nar::ActiveTask::Push::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPush::Request* req) {
    nar::Socket* server_sck;

    try {
        server_sck = this->_globals->establish_server_connection();
    } catch(...) {
        NAR_LOG << "Cannot establish server connection." << std::endl;
        this->send_error_response(ipc_socket, 601);
        return;
    }

    

    std::string file_aes;


    try {
        file_aes = nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Socket::SystemError& Exp) {
        NAR_LOG << "Server connection broken." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 604);
        return;
    } catch(nar::Exception::LowLevelMessaging::Error& Exp) {
        NAR_LOG << "LowLevelMessaging error with server." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 602);
        return;
    } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
        NAR_LOG << "Server sent bad message." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 603);
        return;
    } catch(nar::Exception::Daemon::AuthenticationError& exp) {
        NAR_LOG << "Cannot authenticate " << this->_vars->get_username() << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 702);
        return;
    } catch(nar::Exception::MessageTypes::InternalServerError& exp) {
        NAR_LOG << "Internal server error: " << exp.get_status_code() << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, exp.get_status_code());
        return;
    } catch(...) {
        NAR_LOG << "Unhandled error in authentication." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 900);
        return;
    }


    string aes = file_aes;

    string file_path = req->get_file_path();

    nar::File *original;
    try {
        original = new nar::File(file_path, "r", false);
    } catch(nar::Exception::File::OpenFail& exp) {
        NAR_LOG << "Cannot open file " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 704);
        return;
    }

    nar::File* compressed;


    try {
        compressed = original->compress();
    } catch(nar::Exception::Unknown& exp) {
        NAR_LOG << "Cannot compress file, Exp: {nar::Exception::Unknown} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 900);
        return;
    } catch(nar::Exception::File::OpenFail& exp) {
        NAR_LOG << "Cannot compress file, Exp: {nar::Exception::File::OpenFail} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 705);
        return;
    } catch(nar::Exception::File::CompressError& exp) {
        NAR_LOG << "Cannot compress file, Exp: {nar::Exception::File::CompressError} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 706);
        return;
    } catch(...) {
        NAR_LOG << "Cannot compress file, Exp: {...}  " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 900);
        return;
    }


    nar::File* encrypted;

    try {
        encrypted = compressed->encrypt(aes);
    } catch(nar::Exception::Unknown& exp) {
        NAR_LOG << "Cannot crypt file, Exp: {nar::Exception::Unknown} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 900);
        return;
    } catch(nar::Exception::File::OpenFail& exp) {
        NAR_LOG << "Cannot crypt file, Exp: {nar::Exception::File::OpenFail} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 705);
        return;
    } catch(nar::Exception::File::CryptError& exp) {
        NAR_LOG << "Cannot crypt file, Exp: {nar::Exception::File::CryptError} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 707);
        return;
    } catch(...) {
        NAR_LOG << "Cannot crypt file, Exp: {...} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 900);
        return;
    }


    unsigned long file_size;
    try {
        file_size = encrypted->size();
    } catch(...) {
        NAR_LOG << "Cannot get size, Exp: {...} " << file_path << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 900);
        return;
    }


    std::string pushdir = this->_vars->get_current_directory(); // nar directory


    std::string file_name;
    for(int i=file_path.size()-1; i>=0; i--) {
        if(file_path[i] == '/') break;
        file_name.push_back(file_path[i]);
    }
    std::reverse(file_name.begin(), file_name.end());


    nar::MessageTypes::FilePush::Request push_req(file_name, pushdir, file_size);
    nar::MessageTypes::FilePush::Response push_resp;

    try {
        push_req.send_mess(server_sck, push_resp);
    } catch(nar::Exception::LowLevelMessaging::Error& err) {
        NAR_LOG << "LowLevelMessaging error with server." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 602);
        return;
    } catch(nar::Exception::Socket::SystemError& err) {
        NAR_LOG << "Server connection broken." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 604);
        return;
    } catch(nar::Exception::MessageTypes::NoValidPeerPush& err) { // :(:(
        NAR_LOG << "No valid peer found in the server" << std::endl;
        server_sck->close();
        this->send_error_response(ipc_socket, 707);
        return;
    } catch(nar::Exception::MessageTypes::BadMessageReceive& err) {
        NAR_LOG << "Server sent bad message." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 603);
        return;
    } catch(...) {
        NAR_LOG << "push_req unknown error." << std::endl;
        server_sck->forceclose();
        this->send_error_response(ipc_socket, 900);
        return;
    }


    std::vector<nar::MessageTypes::FilePush::Response::PeerListElement>& elements = push_resp.get_elements();


    unsigned long start = 0;
    for(int i=0; i<elements.size(); i++) {
        long long int current_cid = elements[i].chunk_id;
        int j=i;
        for(; current_cid == elements[j].chunk_id; j++) {
            boost::asio::io_service& ioserv = this->_globals->get_ioserv();
            nar::USocket* usck = new nar::USocket(ioserv, this->_globals->get_server_ip(), push_resp.get_randezvous_port(), elements[j].stream_id);
            usck->connect();
            std::string hash;
            try {
                usck->send(*encrypted, start, elements[i].chunk_size, hash);
            } catch(nar::Exception::USocket::InactivePeer& exp) {
                NAR_LOG << "FAIL. [peer inactive]" << std::endl;
                usck->close();
                server_sck->close();
                this->send_error_response(ipc_socket, 799);
                return;
            }
            usck->close();
        }
        start += elements[i].chunk_size;
        i = j-1;
    }


    nar::MessageTypes::IPCPush::Response ipcpush_resp(200);
    try {
        ipcpush_resp.send_message(ipc_socket);
        ipcpush_resp.send_message_end(ipc_socket);
    } catch(...) {
        NAR_LOG << "IPC Push Response sent unknown error." << std::endl;
    }

    delete compressed;
    delete encrypted;
    delete original;
}
