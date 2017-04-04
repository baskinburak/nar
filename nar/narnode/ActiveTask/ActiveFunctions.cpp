#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <iostream>
#include <string>
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>
using std::string;
using std::cout;
using std::endl;

bool nar::ActiveTask::user_authenticate(nar::Socket* skt, nar::UserVariables* uservars) {
    string user_name = user_vars.get_username();
    string pass_aes = user_vars.get_pass_aes();
    nar::MessageTypes::UserAuthenticationInit::Request init_req(user_name);
    nar::MessageTypes::UserAuthenticationInit::Response init_resp;
    try {
        init_req.send_mess(skt, init_resp);
    }
    catch (nar::Exception::MessageTypes::UserDoesNotExist ex) {
        cout<<"Error in ActiveFunctions "<<ex<<endl;
        return false;
    }
    string fake_private = init_resp.get_private_key();
    string real_private;
    string task_string = init_resp.get_task_string();
    string result_string;
    AesCryptor::AesCryptor aes_crypt(pass_aes);
    aes_crypt.decrypt(fake_private,real_private);
    RsaCryptor::RsaCryptor rsa_crypt;
    rsa_crypt.set_pri(real_private);
    rsa_crypt.decrypt(task_string,result_string);
    nar::MessageTypes::UserAuthenticationAnswer answer_req(result_string);
    nar::MessageTypes::UserAuthenticationAnswer answer_resp;
    answer_req.send_mess(skt, answer_resp);
    try {
        answer_req.send_mess(skt, answer_resp);
    }
    catch(nar::Exception::MessageTypes::ResultStringIsWrong ex) {
        cout<<"Error in ActiveFunctions "<<ex<<endl;
        return false;
    }
    return true;
}
