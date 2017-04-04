#include <nar/narnode/ActiveTask/ActiveTask.h>
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
    nar::Socket* server_sck = this->_globals->establishServerConnection();
    string user_name = this->_vars->get_username();
    string desired_dir = req.get_dir_name();
    nar::MessageTypes::DirInfo::Request dir_req(desired_dir);
    nar::MessageTypes::DirInfo::Response dir_resp;
    if (!nar::ActiveTask::user_authenticate(server_sck, this->_vars)) {
        cout<<"user_authentication fail"<<endl;
    }

    dir_req.send_mess(server_sck, dir_resp);
    std::vector<struct nar::MessageTypes::DirInfo::Response::DirListElement> items = dir_resp.get_elements();
    for(int i =0;i<items.size();i++) {
        std::string entity_name = items[i].entity_name;
        std::string entity_size = std::to_string(items[i].entity_size);
        std::string change_time = items[i].change_time;
        std::string type;
        if(items[i].bool == false) {
            type = std::string(" file");
        } else if(items[i].bool == true) {
            type = std::string(" directory");
        }
        nar::MessageTypes::IPCLs::Response ls_resp(entity_name, entity_size, change_time, type, 100, 200);
        ls_resp.send_message_progress(ipc_socket, 100);

    }
    ls_resp.send_message_end(ipc_socket);
}
