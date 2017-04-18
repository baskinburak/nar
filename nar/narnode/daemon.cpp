#include <iostream>
#include <thread>
#include <nar/lib/Exception/Exception.h>
#include <nar/narnode/utility.h>
#include <cstdlib>
#include <nar/narnode/global.h>
#include <utility>
#include <algorithm>
#include <nar/narnode/uservars.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <nar/narnode/reactive.h>
#include <nar/narnode/ActiveTask/ActiveTask.h>

using std::string;
using std::cout;
using std::endl;

void handle_ipc_request(nar::Socket* sck, nar::Global* globals) {
    std::string msg = nar::trim(nar::get_message(*sck));
    std::string action = nar::Messaging::get_action(msg);
    nar::UserVariables uservars = nar::Messaging::get_user_variables(msg);
    if(action == string("ls")) {
        cout<<"<daemon ls"<<endl;
        nar::MessageTypes::IPCLs::Request ipc_ls;
        ipc_ls.populate_object(msg);
        nar::ActiveTask::LS ls_task(globals, &uservars);
        ls_task.run(sck, &ipc_ls);
        cout << ipc_ls.get_action() << " " << ipc_ls.get_username() << " " << ipc_ls.get_password() << " " << ipc_ls.get_current_directory() << " " << ipc_ls.get_dir_name() << endl;
        cout<<"daemon ls>"<<endl;

    } else if(action == string("push")) {
        nar::MessageTypes::IPCPush::Request ipc_push;
        ipc_push.populate_object(msg);
        nar::ActiveTask::Push push_task(globals, &uservars);
        push_task.run(sck, &ipc_push);
    } else if(action == string("pull")) {
        nar::MessageTypes::IPCPull::Request ipc_pull;
        ipc_pull.populate_object(msg);
        nar::ActiveTask::Pull pull_task(globals, &uservars);
        pull_task.run(sck, &ipc_pull);
    } else if(action == string("register")) {
        nar::MessageTypes::IPCRegister::Request ipc_register;
        ipc_register.populate_object(msg);
        nar::ActiveTask::Register register_task(globals, &uservars);
        register_task.run(sck, &ipc_register);
    } else if(action == string("config")) {
    } else if(action == string("status")) {
    }
}


int main() {
    nar::Global* globals = new nar::Global(std::string("/root/.nar/config"));
    nar::Socket ipc_entry(globals->get_ioserv(), 's');
    ipc_entry.bind(17700, "127.0.0.1");

    std::thread reactive_thr(nar::reactive_dispatcher, globals);
    reactive_thr.detach();

    while(true) {
        nar::Socket* sck = new nar::Socket(globals->get_ioserv(), 'c');
        ipc_entry.accept(*sck);
        std::thread ipc_thread(handle_ipc_request, sck, globals);
        ipc_thread.detach();
    }
}
