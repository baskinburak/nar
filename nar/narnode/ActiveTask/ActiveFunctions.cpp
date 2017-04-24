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
    string user_name = user_vars->get_username();
    string pass_aes = user_vars->get_pass_aes();
    nar::MessageTypes::UserAuthenticationInit::Request init_req(user_name);
    nar::MessageTypes::UserAuthenticationInit::Response init_resp;
    try {
        init_req.send_mess(skt, init_resp);
    }
    catch (nar::Exception::MessageTypes::UserDoesNotExist ex) {
        throw;
    }

    string fake_private = init_resp.get_private_key();
    fake_private = base64_decode(fake_private);
    string real_private;
    string task_string = init_resp.get_task_string();
    task_string = base64_decode(task_string);
    string result_string;
    AesCryptor aes_crypt(pass_aes);
    aes_crypt.decrypt(fake_private,real_private);
    RsaCryptor rsa_crypt;
    rsa_crypt.set_pri(real_private);
    rsa_crypt.decrypt(task_string,result_string);
    result_string = base64_encode((const unsigned char*) result_string.c_str(), result_string.size());
    nar::MessageTypes::UserAuthenticationAnswer::Request answer_req(result_string);
    nar::MessageTypes::UserAuthenticationAnswer::Response answer_resp;
    try {
        answer_req.send_mess(skt, answer_resp);
    }
    catch(nar::Exception::MessageTypes::ResultStringIsWrong ex) {
        throw;
    }

    std::string crypted_aes = init_resp.get_aes_crypted();
    //std::cout << "aes recved enc b64: " << crypted_aes << std::endl;
    crypted_aes = base64_decode(crypted_aes);
    //std::cout << "aes recved enc: " << crypted_aes << std::endl;
    std::string file_aes;
    aes_crypt.decrypt(crypted_aes, file_aes);
    //std::cout << "aes first: " << file_aes << std::endl;
    return file_aes;
}
