#include "ActiveTask.h"
#include <nar/narnode/uservars.h>
#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>
#include <nar/narnode/global.h>
#include <string>

using std::cout;
using std::endl;
using std::string;

/*
 * @author : Dogu
*/


void nar::ActiveTask::Status::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCStatus::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    
    nar::MessageTypes::IPCStatus::Response status_resp(100,200);
    cout<<"received by daemon status"<<endl;
	
    string config_path("/root/.nar");
    const char* m = "r";
    File config_file(config_path.c_str(), m, false);
    char buffer[5000];
    //config_file.read(buffer,0,1024);
	
    int readsize = 0;
    for(int i=0 ; i<config_file.size() ; i+=readsize) {
        readsize = config_file.read(buffer, i, 1024);
        cout << buffer << endl;
    }
	
    status_resp.send_message_progress(ipc_socket, 100);
	
    status_resp.send_message_end(ipc_socket);
}
