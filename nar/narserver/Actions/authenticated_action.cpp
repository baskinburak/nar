#include "ServerActions.h"
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>


void nar::ServerAction::authenticated_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req, nar::Socket* skt) {
    std::string username = req.get_username();
    nar::Database* db = s_global->get_db();
    nar::DBStructs::User user = db->getUser(username);
    if (user.user_id == -1) {                       // NO SUCH USER
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(400, std::string(""), std::string(""), std::string(""));
        return;
    }

    std::string rand_string, task_string;
    AesCryptor::generate_key(rand_string,120);

    RsaCryptor rsa;
    rsa.set_pub(user.rsa_pub);
    rsa.encrypt(rand_string,task_string);

    nar::MessageTypes::UserAuthenticationInit::Response auth_resp(200,user.aes_crypted,user.rsa_pri_crypted,task_string);
    auth_resp.send_mess(skt);

    nar::MessageTypes::UserAuthenticationAnswer::Request ans_req;
    std::string message = get_message(skt);
    ans_req.receive_message(message);
    std::string result_string = ans_req.get_result_string();

    int comp,status_code;
    comp = rand_string.compare(0,rand_string.size(),result_string);
    status_code = 400;
    if(comp == 0) {         // Equeal
        status_code = 200;
    }

    nar::MessageTypes::UserAuthenticationAnswer::Response ans_resp(status_code);
    ans_resp.send_mess(skt);
}
