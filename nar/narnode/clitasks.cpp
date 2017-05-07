#include "clitasks.h"
#include <nar/lib/Messaging/MessageTypes/IPCBaseRequest.h>

#include <nar/lib/Messaging/MessageTypes/IPCBaseResponse.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/lib/Messaging/MessageTypes/IPCPull.h>
#include <nar/lib/Messaging/MessageTypes/IPCPush.h>
#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/lib/Messaging/MessageTypes/IPCStatus.h>
#include <nar/lib/Messaging/MessageTypes/IPCMkdir.h>
#include <nar/lib/Messaging/MessageTypes/IPCDeleteFile.h>


void nar::CLITasks::nar_ls(std::string dir_name, std::string username, std::string password, std::string curdir) {
    nar::MessageTypes::IPCLs::Request req(dir_name, username, password, curdir);

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;

}
void nar::CLITasks::nar_pull(std::string file_name,std::string dir_name, std::string username, std::string password, std::string curdir) {
    std::cout << "File name: " << file_name << std::endl;

    nar::MessageTypes::IPCPull::Request req(file_name,dir_name, username, password, curdir);
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    try {
        ctx.load_verify_file("/root/.nar/ipcserver.crt");
    }
    catch (...) {
        std::cout << "SSL related error, check permissions of ipcserver.crt" << std::endl;
        return;
    }
    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        std::cout << "IPC communication failed for '127.0.0.1::17700'" << std::endl;
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch(nar::Exception::ExcpBase& e ) {
        std::cout << "ipcPullMessage::send: " <<  e.what() << std::endl;
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}

void nar::CLITasks::nar_push(std::string file_name, std::string username, std::string password, std::string curdir) {

	auto co = boost::filesystem::current_path();
	std::string curdirstr = co.string();

	curdirstr += "/"+file_name;
	std::cout << "File name: " << file_name << std::endl;

    MessageTypes::IPCPush::Request req(curdirstr, username, password, curdir);

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}

void nar::CLITasks::nar_register(std::string username, std::string password) {
    MessageTypes::IPCRegister::Request req(username, password, std::string("/"));

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");

    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}

void nar::CLITasks::nar_status() {
    MessageTypes::IPCStatus::Request req(std::string(""), std::string(""), std::string("/"));


    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");


    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}

void nar::CLITasks::nar_delete_file(std::string file_name, std::string username, std::string password, std::string curdir) {
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");

    std::string dir_path;
    std::string file;
    divide_nar_path(file_name,dir_path,file);
    std::cout << file << " " << dir_path << std::endl;
    nar::MessageTypes::IPCDeleteFile::Request req(dir_path,file, username, password, std::string("/"));
    boost::asio::io_service io_serv;
    nar::Socket cli_skt(io_serv, ctx, 'c');
    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}


void nar::CLITasks::nar_mkdir(std::string dir_name, std::string username, std::string password, std::string curdir) {
    std::vector<std::string> names;
    std::size_t found;
    std::size_t last_found;
    std::string temp;
    std::string sql_string = "";
    if((dir_name[dir_name.length()-1] == '/') && dir_name.length()>1 ){
        dir_name = dir_name.substr(0,dir_name.length()-1);
    }
    if((dir_name[0] == '/') && dir_name.length()>1){
        found = dir_name.find("/",1);
        last_found = 1;
    }
    else{
        found = dir_name.find("/");
        last_found = 0;
    }

    while(found != std::string::npos){
        if(found == 0 && last_found == 0){
            temp = dir_name.substr(last_found,1);
            last_found = found+1;
            found = dir_name.find("/",last_found);
            names.push_back(temp);
        }
        else{
            temp = dir_name.substr(last_found,found-last_found);
            last_found = found+1;
            found = dir_name.find("/",last_found);
            names.push_back(temp);
        }

    }
    if(dir_name.length() != 1){
        temp = dir_name.substr(last_found);
        names.push_back(temp);
    }
    std::string dir_path("");
    std::string file_name("");
    for(int i=0;i<names.size();i++) {
        if(i == names.size()-1) {
            file_name = names[i];
        }
        else {
            dir_path += names[i]+ std::string("/");
        }
    }
    std::cout<<"file_name "<<file_name<<std::endl;
    std::cout<<"dir_name "<<dir_path<<std::endl;
    nar::MessageTypes::IPCMkdir::Request req(dir_name,file_name, username, password, std::string("/"));
    boost::asio::io_service io_serv;
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("/root/.nar/ipcserver.crt");
    nar::Socket cli_skt(io_serv, ctx, 'c');

    try {
        cli_skt.connect(std::string("127.0.0.1"), 17700);
    }
    catch(nar::Exception::Socket::ConnectionError er) {
        return;
    }
    try {
        req.send_action(&cli_skt);
        req.print_loop(&cli_skt);
    }
    catch( ... ) {
        std::cout << "Connection lost with daemon" << '\n';
    }
    return;
}
