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
    std::cout<<"1111111111111111111111111111111"<<std::endl;
    if(dest_dir.empty()) {
        dest_dir = _vars->get_current_directory();
    }
    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        std::cout<<exp.what()<<std::endl;
        return;
    }

    std::cout<<"222222222222222222222222222222222"<<std::endl;
    nar::MessageTypes::DeleteFile::Request reqq(file_name,dest_dir);
    nar::MessageTypes::DeleteFile::Response resp;
    try{
        reqq.send_mess(server_sck,resp);
    }
    catch(...){
        std::cout << "Problem in sending" <<std::endl;
        return;
    }
    std::cout<<"3333333333333333333333333333333333"<<std::endl;
    int status = resp.get_status_code();


    nar::MessageTypes::IPCDeleteFile::Response ipc_resp(15,200);
    if(status != 200) {
        ipc_resp.set_status_code(status);
    }
    std::cout<<"11111111111111111111115555555555555551111111111111"<<std::endl;
    ipc_resp.send_message(ipc_socket);
    ipc_resp.send_message_end(ipc_socket);
    std::cout<<"6666666666666666666666666666666666666666"<<std::endl;
}
