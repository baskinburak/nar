#include "ServerActions.h"
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Exception/Exception.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/narserver/Actions/AuthActions.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <nar/lib/Cryption/rsa.h>
#include <nar/lib/base64/base64.h>
#include <iostream>

void nar::ServerAction::authenticate_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req, nar::Socket* skt) {

    std::string& username = req.get_username();

    nar::Database* db = s_global->get_db();
    nar::DBStructs::User user;
    try{
        user = db->getUser(username);
    }
    catch(...) {
        std::cout<<"Server Authentication Database get User Error"<<std::endl;
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(400);
        auth_resp.send_mess(skt);
        return;
    }

    if (user.user_id == -1) {                       // NO SUCH USER
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(300);
        auth_resp.send_mess(skt);
        return;
    }

    std::string rand_string, task_string;
    try{
        AesCryptor::generate_key(rand_string,120);
    } catch(...) {
        std::cout<<"Server Authentication AesCryptor error"<<std::endl;
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(500);
        auth_resp.send_mess(skt);
        return;
    }


    RsaCryptor rsa;
    std::string rsa_pub;
    try {
        rsa_pub = base64_decode(user.rsa_pub);
    } catch(...) {
        std::cout<<"Server Authentication base64 decode error"<<std::endl;
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(501);
        auth_resp.send_mess(skt);
        return;
    }
    try {
        rsa.set_pub(rsa_pub);
        rsa.encrypt(rand_string,task_string);
    } catch(...) {
        std::cout<<"Server Authentication RSA error"<<std::endl;
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(502);
        auth_resp.send_mess(skt);
        return;
    }

    try {
        task_string = base64_encode((const unsigned char*)task_string.c_str(), task_string.size());
    } catch(...) {
        std::cout<<"Server Authentication base64 encode error"<<std::endl;
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(503);
        auth_resp.send_mess(skt);
        return;
    }

    nar::MessageTypes::UserAuthenticationInit::Response auth_resp(200,user.aes_crypted,user.rsa_pri_crypted,task_string);
    try {
        auth_resp.send_mess(skt);
    } catch (nar::Exception::MessageTypes::BadlyConstructedMessageSend exp) {
        std::cout << exp.what() << std::endl;
        auth_resp.set_status_code(504);
        auth_resp.send_mess(skt);
        return;
    }


    nar::MessageTypes::UserAuthenticationAnswer::Request ans_req;
    std::string message;
    try {
        message = get_message(skt);
    } catch(...) {
        std::cout<<"Server Authentication get_message error"<<std::endl;
        return;
    }

    try {
        ans_req.receive_message(message);
    } catch (...) {
        std::cout<<"Server Authentication UserAuthenticationAnswer::Request receive error"<<std::endl;
        nar::MessageTypes::UserAuthenticationAnswer::Response ans_resp(301);
        ans_resp.send_mess(skt);
        return;
    }

    std::string& result_string = ans_req.get_result_string();
    try {
        result_string = base64_decode(result_string);
    } catch(...) {
        std::cout<<"Server Authentication result string base64 decode"<<std::endl;
        nar::MessageTypes::UserAuthenticationAnswer::Response ans_resp(302);
        ans_resp.send_mess(skt);
        return;
    }



    int comp,status_code;
    comp = rand_string.compare(0,rand_string.size(),result_string);
    status_code = 303;
    if(comp == 0) {         // Equal
        status_code = 200;
    }

    nar::MessageTypes::UserAuthenticationAnswer::Response ans_resp(status_code);
    ans_resp.send_mess(skt);
    nar::AuthAction::authentication_dispatcher(s_global,skt,user);
}
