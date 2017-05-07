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
#include <nar/lib/Messaging/MessageTypes/IPCMkdir.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <nar/narnode/reactive.h>
#include <nar/narnode/ActiveTask/ActiveTask.h>

using std::string;
using std::cout;
using std::endl;

void handle_ipc_request(nar::Socket* sck, nar::Global* globals) {
    std::string msg;
    std::string action;
    nar::UserVariables uservars;
    try {
        msg =  nar::trim(nar::get_message(*sck));
        action = nar::Messaging::get_action(msg);
        uservars = nar::Messaging::get_user_variables(msg);
    }
    catch ( nar::Exception::MessageTypes::BadMessageReceive& e ) {
        std::cout << std::string("nar_daemon::handle_ipc_req: ").append(e.what()) << std::endl;
        return;
    }
    catch (...) {
        std::cout << std::string("Undefined error in nar daemon handle ipc req") << std::endl;
        return;
    }
    if(action == string("ls")) {
        cout << "<daemon ls" << endl;
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
        std::cout << "Config not implemented yet! See u soon!" << std::endl;
    } else if(action == string("status")) {
        nar::MessageTypes::IPCStatus::Request ipc_status;
        ipc_status.populate_object(msg);
        nar::ActiveTask::Status status_task(globals, &uservars);
        status_task.run(sck, &ipc_status);
    } else if(action == string("mkdir")) {
        nar::MessageTypes::IPCMkdir::Request ipc_mkdir;
        ipc_mkdir.populate_object(msg);
        nar::ActiveTask::Mkdir mkdir(globals, &uservars);
        mkdir.run(sck, &ipc_mkdir);
    } else if(action == string("delete_file")) {
        nar::MessageTypes::IPCDeleteFile::Request ipc_delete_file;
        ipc_delete_file.populate_object(msg);
        nar::ActiveTask::DeleteFile delete_file(globals, &uservars);
        delete_file.run(sck, &ipc_delete_file);
    }
}


int main() {
    nar::Global* globals = new nar::Global(std::string("/root/.nar/config"));
    nar::Socket ipc_entry(globals->get_ioserv(), globals->get_ipc_ctx(), 's');
    ipc_entry.bind(17700, "127.0.0.1");

    std::thread reactive_thr(nar::reactive_dispatcher, globals);
    reactive_thr.detach();

    while(true) {
        nar::Socket* sck = new nar::Socket(globals->get_ioserv(), globals->get_ipc_ctx(), 'c');
        ipc_entry.accept(*sck);
        std::thread ipc_thread(handle_ipc_request, sck, globals);
        ipc_thread.detach();
    }
}
