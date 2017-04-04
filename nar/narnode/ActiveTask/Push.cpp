#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

void nar::ActiveTask::Push::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req) {
    nar::Socket* server_sck = this->_globals->establishServerConnection();
    if (!nar::ActiveTask::user_authenticate(server_sck, this->_vars)) {
        cout<<"user_authentication fail"<<endl;
    }
    string file_name = req.get_file_name();
    string file_path = _vars->get_current_directory() + file_name;
    /*
        find the RIGHT FILE PATH
    */
    nar::File original(file_path,"r",false);
    nar::File* compressed = original.compress();
    nar::File* encrypted = compressed->encrypt();
    unsigned long long file_size = encrypted->get_size();
    string dir = this->_globals->get_file_folder();
    nar::MessageTypes::FilePush::Request push_req(file_name, dir, file_size);
    nar::MessageTypes::FilePush::Response push_resp;
    push_req.send_mess(server_sck, push_resp);
    


}
