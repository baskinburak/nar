#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/IPCMkdir.h>
#include <nar/lib/Messaging/MessageTypes/Mkdir.h>
using std::cout;
using std::endl;
using std::string;
/*
 * @author : Fatih
 * @todo : try catch  at send message(done by Dogu)
*/

void nar::ActiveTask::Mkdir::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCMkdir::Request* req) {
    nar::Socket* server_sck;
    try{
        server_sck = this->_globals->establish_server_connection();
    } catch(...) {
        NAR_LOG << "Cannot establish server connection." << std::endl;

        nar::MessageTypes::IPCPush::Response resp(601);
        nar::MessageTypes::IPCBaseResponse end_resp;

        try {
            resp.send_message(ipc_socket);
            end_resp.send_message_end(ipc_socket);
        } catch(...) {
            NAR_LOG << "CLI seems down." << std::endl;
        }
        return;
    }

    //server connection is done!
    string dest_dir = req->get_dest_dir(); //harmless
    string dir_name = req->get_dir_name(); //harmless

    if(dest_dir.empty()) { //No-throw guarantee for empty()
        dest_dir = _vars->get_current_directory(); //harmless
    }

    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        NAR_LOG<<exp.what()<<std::endl;
        return;
    }

    nar::MessageTypes::Mkdir::Request mkdir_req(dir_name,dest_dir);

    nar::MessageTypes::Mkdir::Response mdkir_resp;

    try{
        mkdir_req.send_mess(server_sck,mdkir_resp);
    } catch(nar::Exception::ExcpBase e) {
        NAR_LOG << e.what() << std::endl;
        NAR_LOG << "In Mkdir, there is an error related to nar!" << std::endl;
        return;
    } catch(...){
        NAR_LOG << "In Mkdir, there is an error not based on nar!" << std::endl;
        return;
    }
    nar::MessageTypes::IPCMkdir::Response resp;
    try{
        resp.send_message_end(ipc_socket);
    } catch(nar::Exception::LowLevelMessaging::SizeIntOverflow exp) {
        NAR_LOG << exp.what() << std::endl;
    } catch(nar::Exception::LowLevelMessaging::FormatError exp) {
        NAR_LOG << exp.what() << std::endl;
    } catch(nar::Exception::LowLevelMessaging::ServerSizeIntOverflow exp) {
        NAR_LOG << exp.what() << std::endl;
    } catch(...) {
        NAR_LOG << "In Mkdir, There is a problem in send message of utility!" << std::endl;
    }
}
