#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/IPCDeleteFile.h>
#include <nar/lib/Messaging/MessageTypes/DeleteFile.h>
using std::cout;
using std::endl;
using std::string;
/*
 * @author : Fatih
 * @todo : try catch  at send messages
*/

void nar::ActiveTask::DeleteFile::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCDeleteFile::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    string dest_dir = req->get_dest_dir();
    string file_name = req->get_file_name();
    if(dest_dir.empty()) {
        dest_dir = _vars->get_current_directory();
    }
    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        std::cout<<exp.what()<<std::endl;
        return;
    }

    nar::MessageTypes::DeleteFile::Request reqq(file_name,dest_dir);
    nar::MessageTypes::DeleteFile::Response resp;
    try{
        reqq.send_mess(server_sck,resp);
    }
    catch(...){
        return;
    }
    nar::MessageTypes::IPCDeleteFile::Response ipc_resp;
    ipc_resp.send_message_end(ipc_socket);
}
