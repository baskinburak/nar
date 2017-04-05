#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Socket/USocket.h>
#include <iostream>
#include <string>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;


void nar::ActiveTask::Pull::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPush::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    if (!nar::ActiveTask::user_authenticate(server_sck, this->_vars)) {
        cout << "user_authentication fail" << endl;
    }
    string file_name = req.get_file_name();
    string dir_name =  req.get_current_directory();

    nar::MessageTypes::FilePull::Request pull_req(file_name, dir_name);
    nar::MessageTypes::FilePull::Response pull_resp;
    pull_req.send_mess(server_sck, pull_resp);

    std::vector<struct nar::MessageTypes::FilePull::PeerListElement> elements = pull_resp.get_elements();
    std::sort(elements.begin(), elements.end());
    for(int i=0;i<elements.size();i++) {
        
    }
}
