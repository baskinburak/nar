#ifndef NAR_MESS_DAEMON_RECEIVE_H
#define NAR_MESS_DAEMON_RECEIVE_H


#include <string>
#include <iostream>
#include <nar/lib/nlohJson/json.hpp>





#include <nar/lib/Messaging/MessageTypes/IPCConfig.h>
#include <nar/narnode/Task/Config.h>

#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/narnode/Task/LS.h>

#include <nar/lib/Messaging/MessageTypes/IPCPull.h>
#include <nar/narnode/Task/PullFile.h>

#include <nar/lib/Messaging/MessageTypes/IPCPush.h>
#include <nar/narnode/Task/PushFile.h>

#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/narnode/Task/Register.h>

#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>
#include <nar/narnode/Task/Status.h>


#include <nar/lib/Messaging/find_message_action.h>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
namespace nar {
    void daemon_ipc_receive(nar::Socket* skt, nar::Global* globals) {
        string msg = nar::trim(nar::get_message(*skt));
        auto jsn = json::parse(msg.c_str());
        string action = find_message_action(msg);
        if(action == string("ls")) {

            cout<<"<daemon ls"<<endl;
            MessageTypes::IPCLs::Request ipc_ls(string(""));
            ipc_ls.receive_message(jsn);
            cout<<"daemon ls>"<<endl;

        } else if(action == string("push")) {

            cout<<"<daemon push"<<endl;
            MessageTypes::IPCPush::Request ipc_push(string(""));
            ipc_push.receive_message(jsn);
            cout<<"daemon push>"<<endl;

        } else if(action == string("pull")) {

            cout<<"<daemon pull"<<endl;
            MessageTypes::IPCPull::Request ipc_pull(string(""),string(""));
            ipc_pull.receive_message(jsn);
            cout<<"daemon pull>"<<endl;

        } else if(action == string("register")) {

            cout<<"<daemon register"<<endl;
            MessageTypes::IPCRegister::Request ipc_reg(string(""));
            ipc_reg.receive_message(jsn);
            cout<<"daemon register>"<<endl;

        } else if(action == string("config")) {

            cout<<"<daemon config"<<endl;
            MessageTypes::IPCConfig::Request ipc_con(string(""),string(""));
            ipc_con.receive_message(jsn);
            cout<<"daemon config>"<<endl;

        } else if(action == string("status")) {

            cout<<"<daemon status"<<endl;
            MessageTypes::IPCStatus::Request ipc_stat;
            ipc_stat.receive_message(jsn);
            cout<<"daemon status>"<<endl;
        }
        return;
    }
}


#endif
