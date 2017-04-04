#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
using std::cout;
using std::endl;
using std::string;


void nar::ActiveTask::LS::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req) {
    nar::Socket* server_sck = this->_globals->establishServerConnection();
    string user_name = this->_vars->get_username();
    string desired_dir = req.get_dir_name();
    nar::MessageTypes::DirInfo::Request dir_req(desired_dir);
    nar::MessageTypes::DirInfo::Response dir_resp;
    if (!nar::ActiveTask::user_authenticate(server_sck, this->_vars))
}
