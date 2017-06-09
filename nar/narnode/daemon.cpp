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
#include <nar/lib/Messaging/MessageTypes/DaemonShutdown.h>
#include <nar/narnode/reactive.h>
#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <boost/asio/signal_set.hpp>
#include <nar/lib/Logging/logger.h>


nar::Logger *nar_log = new nar::Logger(std::cout);

using std::string;
using std::cout;
using std::endl;

void clean_up(nar::Global* globals, nar::UserVariables* _var) {
    nar::Socket* server_sck = globals->establish_server_connection();
    nar::MessageTypes::DaemonShutdown::Request req(globals->get_machine_id());
    req.send_mess(server_sck);
    return;
}


void handle_ipc_request(nar::Socket* sck, nar::Global* globals) {
    std::string msg;
    std::string action;
    nar::UserVariables uservars;

    try {
        msg = nar::trim(nar::get_message(*sck));
    } catch(nar::Exception::Socket::SystemError& exp) {
        NAR_LOG << "handle_ipc_request: Connection to UI is broken." << std::endl;
        sck->close();
        return;
    } catch(nar::Exception::LowLevelMessaging::NoSize& exp) {
        NAR_LOG << "handle_ipc_request: Low level messaging error: NoSize" << std::endl;
        sck->close();
        return;
    } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow& exp) {
        NAR_LOG << "handle_ipc_request: Low level messaging error: SizeIntOverflow" << std::endl;
        sck->close();
        return;
    } catch(...) {
        NAR_LOG << "handle_ipc_request: Unhandled error" << std::endl;
        sck->close();
        return;
    }

    try {
        action = nar::Messaging::get_action(msg);
    } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
        NAR_LOG << "handle_ipc_request: Bad message received, no action" << std::endl;
        sck->close();
        return;
    }


    try {
        uservars = nar::Messaging::get_user_variables(msg);   //-->>>buraya bak
    } catch ( nar::Exception::MessageTypes::BadMessageReceive& e ) {
        NAR_LOG << "handle_ipc_request: Bad message received, no uservars" << std::endl;
        sck->close();
        return;
    }

    if(action == string("ls")) {
        nar::MessageTypes::IPCLs::Request ipc_ls;
        try {
            ipc_ls.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: ls" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::LS ls_task(globals, &uservars);
        try {
            ls_task.run(sck, &ipc_ls);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("push")) {
        nar::MessageTypes::IPCPush::Request ipc_push;
        try {
            ipc_push.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: push" << std::endl;
            sck->forceclose();
            return;
        }
        nar::ActiveTask::Push push_task(globals, &uservars);
        try {
            push_task.run(sck, &ipc_push);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("pull")) {
        nar::MessageTypes::IPCPull::Request ipc_pull;
        try {
            ipc_pull.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: pull" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::Pull pull_task(globals, &uservars);
        try {
            pull_task.run(sck, &ipc_pull);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("register")) {
        nar::MessageTypes::IPCRegister::Request ipc_register;
        try {
            ipc_register.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: ls" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::Register register_task(globals, &uservars);
        try {
            register_task.run(sck, &ipc_register);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("config")) {
            NAR_LOG << "Config not implemented yet! See u soon!" << std::endl;
    } else if(action == string("status")) {

        nar::MessageTypes::IPCStatus::Request ipc_status;
        try {
            ipc_status.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: ls" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::Status status_task(globals, &uservars);
        try {
            status_task.run(sck, &ipc_status);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("mkdir")) {

        nar::MessageTypes::IPCMkdir::Request ipc_mkdir;
        try {
            ipc_mkdir.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: ls" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::Mkdir mkdir(globals, &uservars);
        try {
            mkdir.run(sck, &ipc_mkdir);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    } else if(action == string("delete_file")) {
        nar::MessageTypes::IPCDeleteFile::Request ipc_delete_file;
        try {
            ipc_delete_file.populate_object(msg);
        } catch(nar::Exception::MessageTypes::BadMessageReceive& exp) {
            NAR_LOG << "handle_ipc_request: Bad message received from UI, Request Type: ls" << std::endl;
            sck->forceclose();
            return;
        }

        nar::ActiveTask::DeleteFile delete_file(globals, &uservars);
        try {
            delete_file.run(sck, &ipc_delete_file);
        } catch(...) {
            NAR_LOG << "handle_ipc_request: push_task.run unhandled error." << std::endl;
            sck->forceclose();
            return;
        }
    }


    //globals->get_ioserv().run();
    sck->close();

}


int main() {
try {
/*    boost::asio::signal_set _signals(globals->get_ioserv());
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
    _signals.async_wait(boost::bind(clean_up, globals, &uservars));*/

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
} catch(std::invalid_argument& ia) {
    NAR_LOG << ia.what() << std::endl;
}
}

