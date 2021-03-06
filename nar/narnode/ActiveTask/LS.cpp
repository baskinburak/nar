#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/lib/Messaging/MessageTypes/DirInfo.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
using std::cout;
using std::endl;
using std::string;

/*
 * @author : Fatih
 * @todo : try catch  at send messages
*/




void nar::ActiveTask::LS::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req) {
    nar::Socket* server_sck;
    try{
        server_sck = this->_globals->establish_server_connection();
    } catch(...) {
        NAR_LOG<<"Daemon ls can not connect to server error"<<std::endl;
        nar::MessageTypes::IPCLs::Response ipc_resp(0,600);
        ipc_resp.send_message_progress(ipc_socket,0);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }
    string &user_name = this->_vars->get_username();
    string desired_dir = req->get_dir_name();
    if(desired_dir.empty()) {
        desired_dir = _vars->get_current_directory();
    }
    nar::MessageTypes::DirInfo::Request dir_req(desired_dir);
    nar::MessageTypes::DirInfo::Response dir_resp;
    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        NAR_LOG<<exp.what()<<std::endl;
        return;
    }
    try {
        dir_req.send_mess(server_sck, dir_resp);
    }  catch(nar::Exception::MessageTypes::BadRequest exp) {
        NAR_LOG<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        nar::MessageTypes::IPCLs::Response ipc_resp(0,exp.get_status_code());
        ipc_resp.send_message_progress(ipc_socket,0);
        ipc_resp.send_message_end(ipc_socket);
        return;
    } catch(nar::Exception::MessageTypes::InternalServerError exp) {
        NAR_LOG<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        nar::MessageTypes::IPCLs::Response ipc_resp(0,exp.get_status_code());
        ipc_resp.send_message_progress(ipc_socket,0);
        ipc_resp.send_message_end(ipc_socket);
        return;
    } catch(nar::Exception::MessageTypes::InternalServerDatabaseError exp) {
        NAR_LOG<<exp.what()<<" status code "<<exp.get_status_code()<<std::endl;
        nar::MessageTypes::IPCLs::Response ipc_resp(0,exp.get_status_code());
        ipc_resp.send_message_progress(ipc_socket,0);
        ipc_resp.send_message_end(ipc_socket);
        return;
    } catch(nar::Exception::MessageTypes::BadMessageReceive exp) {
        NAR_LOG<<exp.what()<<std::endl;
        nar::MessageTypes::IPCLs::Response ipc_resp(0,602);
        ipc_resp.send_message_progress(ipc_socket,0);
        ipc_resp.send_message_end(ipc_socket);
        return;
    }

    std::vector<struct nar::MessageTypes::DirInfo::Response::DirListElement>& items = dir_resp.get_elements();
    nar::MessageTypes::IPCLs::Response ls_resp;
    for(int i =0;i<items.size();i++) {
        std::string entity_name = items[i].entity_name;
        std::string entity_size = std::to_string(items[i].entity_size);
        std::string change_time = items[i].change_time;
        std::string type;
        if( !items[i].type ) {
            type = std::string(" file");
        } else {
            type = std::string(" directory");
        }
        ls_resp.set_entity_name(entity_name);
        ls_resp.set_entity_size(entity_size);
        ls_resp.set_change_time(change_time);
        ls_resp.set_type(type);


        ls_resp.send_message_progress(ipc_socket, 100);

    }
    ls_resp.send_message_end(ipc_socket);
}
