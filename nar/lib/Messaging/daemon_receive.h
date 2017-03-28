#ifndef NAR_MESS_DAEMON_RECEIVE_H
#define NAR_MESS_DAEMON_RECEIVE_H


#include <string>
#include <iostream>
#include <nar/lib/nlohJson/json.hpp>

#include <nar/lib/Messaging/MessageTypes/IPCConfig.h>

#include <nar/lib/Messaging/MessageTypes/IPCLs.h>

#include <nar/lib/Messaging/MessageTypes/IPCPull.h>

#include <nar/lib/Messaging/MessageTypes/IPCPush.h>

#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>

#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>


#include <nar/lib/Messaging/find_message_action.h>


using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
namespace nar {
    void daemon_receive(nar::Socket* skt, nar::Global* globals) {
        string msg = nar::trim(nar::get_message(*skt));
        string action = find_message_action(msg);
        return;
    }



}


#endif
