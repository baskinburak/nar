#include "clitasks.h"
#include <nar/lib/Messaging/MessageTypes/IPCBaseRequest.h>

#include <nar/lib/Messaging/MessageTypes/IPCBaseResponse.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/lib/Messaging/MessageTypes/IPCPull.h>
#include <nar/lib/Messaging/MessageTypes/IPCPush.h>
#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>



void nar::CLITasks::nar_ls(std::string dir_name, std::string username, std::string password, std::string curdir) {
    nar::MessageTypes::IPCLs::Request req(dir_name, username, password, curdir);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;

}
void nar::CLITasks::nar_pull(std::string file_name, std::string username, std::string password, std::string curdir) {
    std::cout << "File name: " << file_name << std::endl;

    nar::MessageTypes::IPCPull::Request req(file_name, username, password, curdir);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;
}

void nar::CLITasks::nar_push(std::string file_name, std::string username, std::string password, std::string curdir) {

	auto co = boost::filesystem::current_path();
	std::string curdirstr = co.string();

	curdirstr += "/"+file_name;
	std::cout << "File name: " << file_name << std::endl;

    MessageTypes::IPCPush::Request req(curdirstr, username, password, curdir);

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;
}

void nar::CLITasks::nar_register(std::string username, std::string password) {
    MessageTypes::IPCRegister::Request req(username, password, std::string("/"));

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;
}

void nar::CLITasks::nar_status() {
    MessageTypes::IPCStatus::Request req(std::string(""), std::string(""), std::string("/"));

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, 'c');
    cli_skt.connect(std::string("127.0.0.1"), 17700);

    req.send_action(&cli_skt);
    req.print_loop(&cli_skt);
    return;
}
