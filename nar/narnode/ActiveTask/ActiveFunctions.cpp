#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>
#include <iostream>
#include <string>
#include <nar/lib/base64/base64.h>

using std::string;
using std::cout;
using std::endl;

std::string nar::ActiveTask::user_authenticate(nar::Socket* skt, nar::UserVariables* user_vars) {
    string& user_name = user_vars->get_username();
    string& pass_aes = user_vars->get_pass_aes();
    nar::MessageTypes::UserAuthenticationInit::Request init_req(user_name);
    nar::MessageTypes::UserAuthenticationInit::Response init_resp;
    try {
        init_req.send_mess(skt, init_resp);
    }
    catch(nar::Exception::MessageTypes::BadRequest exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::InternalServerError exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::InternalServerDatabaseError exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::BadMessageReceive exp) {
        std::cout<<exp.what()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    string& fake_private = init_resp.get_private_key();
    string real_private;
    string& task_string = init_resp.get_task_string();
    string result_string;
    try{
        fake_private = base64_decode(fake_private);

        task_string = base64_decode(task_string);

    } catch(...) {
        std::cout<<"Daemon authentication base 64 decode error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    AesCryptor aes_crypt(pass_aes);
    RsaCryptor rsa_crypt;
    try {
        aes_crypt.decrypt(fake_private,real_private);
    } catch (...) {
        std::cout<<"Daemon authentication aes decrypt error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    try {
        rsa_crypt.set_pri(real_private);
        rsa_crypt.decrypt(task_string,result_string);
    } catch (...) {
        std::cout<<"Daemon authentication rsa set_pri or decrypt error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    try{
        result_string = base64_encode((const unsigned char*) result_string.c_str(), result_string.size());
    } catch (...) {
        std::cout<<"Daemon authentication base 64 encode error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }
    nar::MessageTypes::UserAuthenticationAnswer::Request answer_req(result_string);
    nar::MessageTypes::UserAuthenticationAnswer::Response answer_resp;
    try {
        answer_req.send_mess(skt, answer_resp);
    }
    catch(nar::Exception::MessageTypes::BadRequest exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::InternalServerError exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::InternalServerDatabaseError exp) {
        std::cout<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    } catch(nar::Exception::MessageTypes::BadMessageReceive exp) {
        std::cout<<exp.what()<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    std::string& crypted_aes = init_resp.get_aes_crypted();
    //std::cout << "aes recved enc b64: " << crypted_aes << std::endl;
    try {
        crypted_aes = base64_decode(crypted_aes);
    } catch (...) {
        std::cout<<"Daemon authentication base 64 decode error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    //std::cout << "aes recved enc: " << crypted_aes << std::endl;
    std::string file_aes;
    try{
        aes_crypt.decrypt(crypted_aes, file_aes);
    } catch (...) {
        std::cout<<"Daemon authentication aes decrypt error"<<std::endl;
        throw nar::Exception::Daemon::AuthenticationError("Authentication is not successful");
    }

    //std::cout << "aes first: " << file_aes << std::endl;
    return file_aes;
}
