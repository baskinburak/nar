#include "ActiveTask.h"
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>
#include <nar/narnode/uservars.h>
#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/narnode/global.h>
#include <string>
#include <nar/lib/base64/base64.h>

void nar::ActiveTask::Register::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCRegister::Request* req) {
    std::string& username = this->_vars->get_username();
    std::string& pass_aes = this->_vars->get_pass_aes();

    std::string aes;
    std::string crypted_aes;
    AesCryptor aes_cryptor(pass_aes);
    try {
        AesCryptor::generate_key(aes, 16);
        //std::cout << "first key: " << aes << std::endl;




        aes_cryptor.encrypt(aes, crypted_aes);

        //std::cout << "first key enc: " << crypted_aes << std::endl;
    }
    catch(...) {
        std::cout<<"Daemon register aes cryptor error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,600);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }

    std::string b64_crypted_aes;
    try{
        b64_crypted_aes= base64_encode((const unsigned char*) crypted_aes.c_str(), crypted_aes.size());
    } catch(...) {
        std::cout<<"Daemon register base 64 encode error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,601);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }


    //std::cout << "first key enc b64: "<< b64_crypted_aes << std::endl;

    std::string pri, pub;
    try {
        RsaCryptor::generate_key_pair(pub, pri);
    } catch(...) {
        std::cout<<"Daemon register RSACryptor generate key pair error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,602);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }
    std::string b64_pub;
    try {
        b64_pub = base64_encode((const unsigned char*) pub.c_str(), pub.size());
    }
    catch(...) {
        std::cout<<"Daemon register base 64 encode error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,603);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }
    std::string crypted_pri_key;
    try {
        aes_cryptor.encrypt(pri, crypted_pri_key);
    } catch(...) {
        std::cout<<"Daemon register aes cryptor encrypt error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,604);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }
    std::string b64_crypted_pri_key;
    try{
        b64_crypted_pri_key = base64_encode((const unsigned char*) crypted_pri_key.c_str(), crypted_pri_key.size());
    } catch(...) {
        std::cout<<"Daemon register base 64 encode error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,605);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }



    nar::MessageTypes::UserRegister::Request request(username, b64_crypted_aes, b64_pub, b64_crypted_pri_key);
    nar::Socket* srvsck;
    try {
        srvsck= _globals->establish_server_connection();
    } catch(...) {
        std::cout<<"Daemon register can not connect to server error"<<std::endl;
        nar::MessageTypes::IPCRegister::Response ipc_resp(0,606);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }


    nar::MessageTypes::UserRegister::Response resp;////// todo:aaaaaaaaaaaaaaaaaaaaaa
    try {
        request.send_mess(srvsck, resp);
    } catch(...) {

    } catch(...) {

    } catch(...) {

    }

}
void nar::ActiveTask::Register::run( nar::MessageTypes::IPCRegister::Request* req) {
    std::string& username = this->_vars->get_username();
    std::string& pass_aes = this->_vars->get_pass_aes();

    std::string aes;
    AesCryptor::generate_key(aes, 16);

    //std::cout << "first key: " << aes << std::endl;

    AesCryptor aes_cryptor(pass_aes);

    std::string crypted_aes;
    aes_cryptor.encrypt(aes, crypted_aes);

   // std::cout << "first key enc: " << crypted_aes << std::endl;

    std::string b64_crypted_aes = base64_encode((const unsigned char*) crypted_aes.c_str(), crypted_aes.size());

   // std::cout << "first key enc b64: "<< b64_crypted_aes << std::endl;

    std::string pri, pub;
    RsaCryptor::generate_key_pair(pub, pri);

    std::string b64_pub = base64_encode((const unsigned char*) pub.c_str(), pub.size());

    std::string crypted_pri_key;
    aes_cryptor.encrypt(pri, crypted_pri_key);

    std::string b64_crypted_pri_key = base64_encode((const unsigned char*) crypted_pri_key.c_str(), crypted_pri_key.size());


    nar::MessageTypes::UserRegister::Request request(username, b64_crypted_aes, b64_pub, b64_crypted_pri_key);
    nar::Socket* srvsck = _globals->establish_server_connection();

    nar::MessageTypes::UserRegister::Response resp;
    request.send_mess(srvsck, resp);
}
