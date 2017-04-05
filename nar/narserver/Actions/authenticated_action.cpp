#include "ServerActions.h"
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>


void nar::ServerAction::authenticated_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req, nar::Socket* skt) {
    std::string username = req.get_username();
    nar::Database* db = s_global->get_db();
    nar::User user = db->getUser(username);
    if (user.user_id == -1) {                       // NO SUCH USER
        nar::MessageTypes::UserAuthenticationInit::Response auth_resp(400,std::string("bla"),std::string("bla"));
        return;
    }

    std::string rand_string, task_string;
    AesCryptor::generate_key(rand_string,120);

    RsaCryptor rsa();
    rsa.set_pub(user.rsa_pub);
    rsa.encrypt(rand_string,task_string);
    nar::MessageTypes::UserAuthenticationInit::Response auth_resp(200,user.aes_crypted,user.rsa_pri_crypted,task_string);
    auth_resp.send_mess();

}
