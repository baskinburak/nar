#include "ServerActions.h"
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>

void nar::ServerAction::register_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserRegister::Request& req, nar::Socket* skt) {
    nar::Database* db = s_global->get_db();
    nar::DBStructs::User usr;
    usr.user_name = req.get_username();
    usr.aes_crypted = req.get_aes_crypted();
    usr.rsa_pub = req.get_rsa_pub();
    usr.rsa_pri_crypted = req.get_rsa_pri_crypted();

    db->insertUser(usr);

    nar::MessageTypes::UserRegister::Response resp(200);
    resp.send_mess(skt);
}
