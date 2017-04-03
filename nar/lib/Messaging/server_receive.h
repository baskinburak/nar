#ifndef NAR_MESS_SERVER_RECEIVE_H
#define NAR_MESS_SERVER_RECEIVE_H

#include <string>
#include <iostream>
#include <nar/narserver/ServerGlobal.h>
#include <nar/narserver/Actions/handshake.h>
#include <nar/lib/Messaging/MessageTypes/Handshake.h>

#include <nar/narserver/Actions/user_register.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>

#include <nar/narserver/Actions/machine_register.h>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>

#include <nar/narserver/Actions/keepalive.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>

#include <nar/narserver/Actions/dir_info.h>
#include <nar/lib/Messaging/MessageTypes/DirInfo.h>

#include <nar/narserver/Actions/aes_key.h>
#include <nar/lib/Messaging/MessageTypes/AesKey.h>

#include <nar/narserver/Actions/file_push.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>

#include <nar/narserver/Actions/file_pull.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>

#include <nar/lib/nlohJson/json.hpp>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
namespace nar {
    /*
     * Received messages are forwarded to their respected functions
     *
     * @author: Fatih
     * @param: skt, nar::Socket*,
    */
    void server_receive(nar::Socket* skt, nar::ServerGlobal* s_global){
        nar::Database* db = s_global->get_db();
        bool handshaked = false;
        while(true) {
            string msg = nar::trim(nar::get_message(*skt));
            auto jsn = json::parse(msg.c_str());
            if(jsn["header"]["action"] == "handshake") {
                // user info
                cout<<"<handshake"<<endl;
                MessageTypes::Handshake::Request hand_req;
                hand_req.receive_message(jsn);
                handshaked = ServerActions::handshake(inf,hand_req);

                cout<<"handshake>"<<endl;
            } else if(jsn["header"]["action"] == "machine_register") {
                cout<<"<machine_register"<<endl;

                MessageTypes::MachineRegister::Request macreg_req;
                macreg_req.receive_message(jsn);
                ServerActions::machine_register(inf, macreg_req, db);

                cout<<"machine_register>"<<endl;
            } else if(jsn["header"]["action"] == "keepalive") {
                cout<<"<keepalive"<<endl;
                MessageTypes::KeepAlive::Request keep_req;
                keep_req.receive_message(jsn);
                ServerActions::keepalive(inf,keep_req ,s_global);

                cout<<"keepalive>"<<endl;
            } else if(jsn["header"]["action"] == "user_register") {
                cout<<"<user_register"<<endl;

                MessageTypes::UserRegister::Request usreg_req(string(""),string(""));
                usreg_req.receive_message(jsn);
                ServerActions::user_register(inf, usreg_req, db);

                cout<<"user_register>"<<endl;

            } else if(jsn["header"]["action"] == "get_dir_info") {
                cout<<"<dir_info"<<endl;

                MessageTypes::DirInfo::Request dir_req(string(""));
                dir_req.receive_message(jsn);
                ServerActions::get_user_dir_info(inf, dir_req, db);

                cout<<"dir_info>"<<endl;

            } else if(jsn["header"]["action"] == "get_aes_key") {
                cout<<"<aes_key"<<endl;

                MessageTypes::AesKey::Request aes_req;
                aes_req.receive_message(jsn);
                ServerActions::get_aes_key(inf, aes_req, db);

                cout<<"aes_key>"<<endl;

            } else if(jsn["header"]["action"] == "file_push") {
                cout<<"<file_push_key"<<endl;

                MessageTypes::FilePush::Request fpush_req(string(""),string(""),-1);
                fpush_req.receive_message(jsn);
                ServerActions::file_push(inf, fpush_req, db, s_global);

                cout<<"file_push_key>"<<endl;

            } else if(jsn["header"]["action"] == "file_pull") {
                cout<<"<file_pull_key"<<endl;

                MessageTypes::FilePull::Request fpull_req(string(""),string(""));
                fpull_req.receive_message(jsn);
                ServerActions::file_pull(inf, fpull_req, db, s_global);

                cout<<"file_pull_key>"<<endl;

            }


        }
        if(inf->isKeepalive()) {
        } else {
            (inf->getSck())->close();
        }

    }
}

#endif
