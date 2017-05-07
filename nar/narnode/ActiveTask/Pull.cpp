#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Socket/USocket.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>

using std::string;
using std::cout;
using std::endl;


// TO_DO: Last part should handled for file::encrypt,decompress etc..

void nar::ActiveTask::Pull::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPull::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    std::string file_aes;
    try {
        file_aes = nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError& exp) {
        std::cout<<exp.what()<<std::endl;
        return;
    }
    string file_name,system_dir,nar_dir_name;
    try {
        file_name = req->get_file_name();
        system_dir = req->get_dir_path(); // to be changed
        nar_dir_name =  req->get_current_directory();
    }
    catch (...) {
        std::cout << "Malformed Json, exiting pull" << std::endl;
        return;
    }

    nar::MessageTypes::FilePull::Request pull_req(file_name, nar_dir_name);
    nar::MessageTypes::FilePull::Response pull_resp;
    try {
        pull_req.send_mess(server_sck, pull_resp);
    }
    catch (nar::Exception::ExcpBase& e) {
        std::cout << std::string( "nar_daemon::activepull " ).append(e.what()) << std::endl;
        return;
    }
    unsigned short rand_port = pull_resp.get_rendezvous_port();
    std::cout << "resp: " << rand_port << std::endl;
    std::vector<struct nar::MessageTypes::FilePull::Response::PeerListElement> elements = pull_resp.get_elements();
    std::sort(elements.begin(), elements.end());

    std::cout << "After pull req" << std::endl;

    boost::filesystem::path tpath(system_dir);
    tpath /= file_name;

    std::cout << tpath.string() << std::endl;


    boost::filesystem::path temp;
    try {
        temp = boost::filesystem::unique_path();
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
    std::string temp_native = temp.native();

    nar::File* tempfile1 = new nar::File(temp_native,"w",false);

    try {
        for(int i=0;i<elements.size();i++) {
            unsigned long stream_id = elements[i].stream_id;
            std::cout << "rand_port: " << rand_port << endl;
            nar::USocket* cli_sck = new nar::USocket(this->_globals->get_ioserv(), this->_globals->get_server_ip(), rand_port, stream_id);
            cli_sck->connect();
            long int total_read = 0;
            char buf[1024];
            while(total_read < elements[i].chunk_size) {
                int len = cli_sck->recv(buf, 1024);
                tempfile1->write(buf,len);
                total_read += len;
            }
            cli_sck->close();
        }
    }
    catch ( ... ) {
        std::cout << "Problem occured when pulling chunks from peers" << std::endl;
        delete tempfile1;
        return;
    }
    delete tempfile1;
    tempfile1 = new nar::File(temp_native,"r",true);

    nar::File * decrypted = tempfile1->decrypt(file_aes);

    string  dust= tpath.native();
    nar::File * decompressed = decrypted->decompress(dust);


    nar::MessageTypes::IPCPull::Response ipcpull_resp(3,5);

    ipcpull_resp.send_message_end(ipc_socket);

    delete tempfile1;
    delete decrypted;
    delete decompressed;
}
