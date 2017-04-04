#include "clitasks.h"
#include <nar/lib/Messaging/MessageTypes/IPCBaseRequest.h>

#include <nar/lib/Messaging/MessageTypes/IPCBaseResponse.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>



void nar::CLITasks::nar_ls(std::string dir_name, std::string username, std::string password, std::string curdir) {
    MessageTypes::IPCLs::Request req(dir_name, username, password, curdir);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;

}
void nar::CLITasks::nar_pull(std::string file_name, std::string username, std::string password, std::string curdir) {
    std::cout << file_name << std::endl;

    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        //Exception
        std::cout << "could not get current directory" << std::endl;
    }
    std::string my_cur_dir(cwd);

    MessageTypes::IPCPull::Request req(file_name, my_cur_dir);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;
}

void nar::CLITasks::nar_push(std::string file_name, std::string username, std::string password, std::string curdir) {
    /*char* real_path = realpath(file_name.c_str(), NULL);
    std::string fileName(std::string(real_path));
    std::cout << fileName << std::endl;

    std::ifstream f(fileName.c_str());
    if(!f.good()) {
        std::cout << fileName  << " could not be opened." << std::endl;
        return;
    }

    MessageTypes::IPCStatus::Request req(fileName);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 23456);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;*/
}

void nar::CLITasks::nar_register(std::string username, std::string password) {
    /*MessageTypes::IPCRegister::Request req(user_name);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 23456);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;*/

}

void nar::CLITasks::nar_status() {
    /*MessageTypes::IPCStatus::Request req;

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 23456);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;*/

}
