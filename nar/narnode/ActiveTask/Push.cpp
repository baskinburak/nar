#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

void nar::ActiveTask::Push::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPush::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    if (!nar::ActiveTask::user_authenticate(server_sck, this->_vars)) {
        cout << "user_authentication fail" << endl;
    }
    string file_name = req->get_file_name();
    string file_path = _vars->get_current_directory() + file_name;

    nar::File original(file_path, "r", false);
    nar::File* compressed = original.compress();
    nar::File* encrypted = compressed->encrypt();

    unsigned long long file_size = encrypted->get_size();
    string pushdir = this->_vars->get_current_directory();

    nar::MessageTypes::FilePush::Request push_req(file_name, pushdir, file_size);
    nar::MessageTypes::FilePush::Response push_resp;
    push_req.send_mess(server_sck, push_resp);
}
