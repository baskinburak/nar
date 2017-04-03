#ifndef NAR_SERVER_ACTIONS_AES_KEY_H
#define NAR_SERVER_ACTIONS_AES_KEY_H


#include <nar/lib/Messaging/MessageTypes/AesKey.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/sockinfo.h>
#include <iostream>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::string;

namespace nar {
        namespace ServerActions {
            void get_aes_key(nar::SockInfo* inf, MessageTypes::AesKey::Request & aes_req, nar::Database* db) {
                if(inf->isAuthenticated()) {
                    std::string user_name = inf->getUser();
                    nar::User usr = db->getUser(user_name);
                    if(usr.user_id != -1) {
                        nar::MessageTypes::AesKey::Response aes_resp(200,usr.cryptedKey);
                        aes_resp.send_mess(inf->getSck());
                        return;
                    } else {
                        nar::MessageTypes::AesKey::Response aes_resp(301,std::string(""));
                        aes_resp.send_mess(inf->getSck());
                        return;
                    }
                } else {
                    nar::MessageTypes::AesKey::Response aes_resp(300,std::string(""));
                    aes_resp.send_mess(inf->getSck());
                    return;
                }

            }
        }
}


#endif
