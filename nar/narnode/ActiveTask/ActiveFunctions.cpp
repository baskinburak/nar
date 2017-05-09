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
    } catch(nar::Exception::Authentication::NoSuchUsername& exp) {
        throw nar::Exception::Daemon::AuthenticationError("NoSuchUsername");
    }

    string& fake_private = init_resp.get_private_key();
    string real_private;
    string& task_string = init_resp.get_task_string();
    string result_string;

    try{
        fake_private = base64_decode(fake_private);
        task_string = base64_decode(task_string);
    } catch(...) {
        throw nar::Exception::Daemon::AuthenticationError("base64 decode error.");
    }

    AesCryptor aes_crypt(pass_aes);
    RsaCryptor rsa_crypt;

    try {
        aes_crypt.decrypt(fake_private,real_private);
    } catch (...) {
        throw nar::Exception::Daemon::AuthenticationError("AES decrypt is not successful.");
    }

    try {
        rsa_crypt.set_pri(real_private);
        rsa_crypt.decrypt(task_string,result_string);
    } catch (...) {
        throw nar::Exception::Daemon::AuthenticationError("RSA decrypt is not successful.");
    }

    try {
        result_string = base64_encode((const unsigned char*) result_string.c_str(), result_string.size());
    } catch (...) {
        throw nar::Exception::Daemon::AuthenticationError("base64 encode error.");
    }



    nar::MessageTypes::UserAuthenticationAnswer::Request answer_req(result_string);
    nar::MessageTypes::UserAuthenticationAnswer::Response answer_resp;
    try {
        answer_req.send_mess(skt, answer_resp);
    } catch(nar::Exception::Authentication::WrongTaskString& exp) {
        throw nar::Exception::Daemon::AuthenticationError("WrongTaskString");
    }

    std::string& crypted_aes = init_resp.get_aes_crypted();
    try {
        crypted_aes = base64_decode(crypted_aes);
    } catch (...) {
        throw nar::Exception::Daemon::AuthenticationError("base64 decode error");
    }


    std::string file_aes;
    try{
        aes_crypt.decrypt(crypted_aes, file_aes);
    } catch (...) {
        throw nar::Exception::Daemon::AuthenticationError("AES decrypt file_aes error.");
    }

    //std::cout << "aes first: " << file_aes << std::endl;
    return file_aes;
}
