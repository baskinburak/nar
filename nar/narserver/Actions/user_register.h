#ifndef NAR_SERVER_ACTIONS_USER_REGISTER_H
#define NAR_SERVER_ACTIONS_USER_REGISTER_H

#include <iostream>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/narserver/sockinfo.h>
#include <string>


using std::cout;
using std::endl;

namespace nar {
    namespace ServerActions {
        void user_register(nar::SockInfo* inf, MessageTypes::UserRegister::Request & usreg_req, nar::Database* db) {
            std::string username = usreg_req.get_username();
            std::string aes = usreg_req.get_aes();
            nar::User usr = db->getUser(username);
            int status_code;
            if(usr.user_id != -1) {
                status_code = 300; // username already exists
            }

            usr.user_name = username;
            usr.quota = 0;
            usr.disk_space = 0;
            usr.cryptedKey = aes;
            db->insertUser(usr);
            status_code = 200; // success
            nar::MessageTypes::UserRegister::Response usreg_resp(status_code);
            usreg_resp.send_mess(inf->getSck());
            return;
        }
    }
}


#endif
