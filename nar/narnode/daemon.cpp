#include <iostream>
#include <thread>
#include <nar/lib/Exception/exception.h>
#include <nar/narnode/utility.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <cstdlib>
#include <nar/narnode/global.h>
#include <nar/lib/nlohJson/json.hpp>
#include <utility>
#include <algorithm>
#include <nar/narnode/uservars.h>
#include <nar/narnode/reactive.h>

void handle_ipc_request(nar::Socket* sck, nar::Global* globals) {
    std::string msg = nar::trim(nar::get_message(*skt));
    auto jsn = json::parse(msg.c_str());
    std::string action = msg["header"]["action"];
    UserVariables uservars(msg["header"]["current_directory"], msg["header"]["username"], msg["header"]["password"]);
    if(action == string("ls")) {
        cout<<"<daemon ls"<<endl;
        nar::MessageTypes::IPCLs::Request ipc_ls;
        ipc_ls.receive_message(jsn);
        cout<<"daemon ls>"<<endl;

    } else if(action == string("push")) {

        cout<<"<daemon push"<<endl;
        nar::MessageTypes::IPCPush::Request ipc_push(string(""));
        ipc_push.receive_message(jsn);
        cout<<"daemon push>"<<endl;

    } else if(action == string("pull")) {

        cout<<"<daemon pull"<<endl;
        nar::MessageTypes::IPCPull::Request ipc_pull(string(""),string(""));
        ipc_pull.receive_message(jsn);
        cout<<"daemon pull>"<<endl;

    } else if(action == string("register")) {

        cout<<"<daemon register"<<endl;
        nar::MessageTypes::IPCRegister::Request ipc_reg(string(""));
        ipc_reg.receive_message(jsn);
        cout<<"daemon register>"<<endl;

    } else if(action == string("config")) {

        cout<<"<daemon config"<<endl;
        nar::MessageTypes::IPCConfig::Request ipc_con(string(""),string(""));
        ipc_con.receive_message(jsn);
        cout<<"daemon config>"<<endl;

    } else if(action == string("status")) {

        cout<<"<daemon status"<<endl;
        nar::MessageTypes::IPCStatus::Request ipc_stat;
        ipc_stat.receive_message(jsn);
        cout<<"daemon status>"<<endl;
    }
}


int main() {
    nar::Global* globals = new nar::Global("/root/.nar/config");
    nar::Socket ipc_entry(globals->get_ioserv(), 's');
    ipc_entry.bind(17700, "lo");

    std::thread reactive_thr(nar::reactive_dispatcher, globals);
    reactive_thr.detach();


    while(true) {
        nar::Socket* sck = new nar::Socket(globals->get_ioserv, 'c');
        ipc_entry.accept(*sck);
        std::thread ipc_thread(handle_ipc_request, sck, globals);
        ipc_thread.detach();
    }
}
