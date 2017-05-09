#include "ActiveTask.h"
#include <nar/narnode/uservars.h>
#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>
#include <nar/narnode/global.h>
#include <string>
#include "boost/filesystem/fstream.hpp"

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

using std::cout;
using std::endl;
using std::string;

/*
 * @author : Dogu
*/


void nar::ActiveTask::Status::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCStatus::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    //std::cout << "statuqo" << std::endl;
    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        std::cout<<exp.what()<<std::endl;
        return;
    }


    cout<<"received by daemon status"<<endl;

    boost::filesystem::path path(this->_globals->get_nar_folder());
    boost::filesystem::path config_path( path.string() + "config" );
    //std::cout << config_path << std::endl;
    const char* m = "r";
    File config_file(config_path.c_str(), m, false);
    boost::filesystem::ifstream fin(config_path);


    char buffer[5000];
    //config_file.read(buffer,0,1024);

    std::string var_arr[5];
    for(int i=0 ; i<5 ; i++){
        std::getline(fin, var_arr[i]);
        //std::string tmp = var_arr[i].substr(var_arr[i].find("=") + 1);
        //std::cout << tmp << std::endl;
        //var_arr[i] = tmp;
    }

    nar::MessageTypes::IPCStatus::Response status_resp(100, 200, var_arr[0], var_arr[1], var_arr[2], var_arr[3], var_arr[4]);

    status_resp.send_message(ipc_socket);

    status_resp.send_message_end(ipc_socket);

}
