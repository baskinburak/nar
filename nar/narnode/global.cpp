#include <nar/narnode/global.h>
#include <iostream>
#include <nar/narnode/uservars.h>
#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>


void nar::Global::read_start() {
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::Global::read_end() {
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::Global::write_start() {
    write_mtx.lock();
}

void nar::Global::write_end() {
    write_mtx.unlock();
}

nar::Global::Global(std::string config_path): _server_ip(std::string()), _server_port(0), _nar_folder(std::string()), _file_folder(std::string()), _machine_id(std::string()), _config_path(config_path) {
    _config_file.open(config_path.c_str(), std::fstream::in | std::fstream::out);
    std::string line;
    char mask = 0;
    while(std::getline(_config_file, line)) {
        std::string key;
        std::string value;
        int i;
        for(i=0; i<line.size(); i++) {
            if(line[i] == '=')
                break;
            key.push_back(line[i]);
        }
        i++;
        for(; i<line.size(); i++)
            value.push_back(line[i]);

        if(key == "NAR_FOLDER") {
            mask |= 1;
            this->_nar_folder = value;
        } else if(key == "FILE_FOLDER") {
            mask |= 2;
            this->_file_folder = value;
        } else if(key == "SERVER_IP") {
            mask |= 4;
            this->_server_ip = value;
        } else if(key == "SERVER_PORT") {
            mask |= 8;
            std::cout << value << " <<<< " << std::endl;
            this->_server_port = std::stoi(value);
            std::cout << this->_server_port << std::endl;
        } else if(key == "MACHINE_ID") {
            mask |= 16;
            this->_machine_id = value;
            if (this->_machine_id == std::string("")) {
                this->_machine_id = machine_register();
                std::cout << "machine id: " << this->_machine_id << std::endl;
            }
        }
    }

    this->_config_file.close();

    this->write_config();

    if(mask != (1 | 2 | 4 | 8 | 16)) {
        std::cout << "Config file is not complete" << std::endl;
        exit(0);
    }
}

nar::Global::~Global() {
    this->write_config();
    this->_config_file.close();
}

std::string nar::Global::get_server_ip() {
    read_start();
    std::string ret = this->_server_ip;
    read_end();
    return ret;
}
unsigned short nar::Global::get_server_port() {
    read_start();
    unsigned short ret = this->_server_port;
    read_end();
    return ret;
}
std::string nar::Global::get_machine_id() {
    read_start();
    std::string ret = this->_machine_id;
    read_end();
    return ret;
}
std::string nar::Global::get_nar_folder() {
    read_start();
    std::string ret = this->_nar_folder;
    read_end();
    return ret;
}
std::string nar::Global::get_file_folder() {
    read_start();
    std::string ret = this->_file_folder;
    read_end();
    return ret;
}
boost::asio::io_service& nar::Global::get_ioserv() {
    return this->_ioserv;
}

void nar::Global::set_server_ip(const std::string& ip) {
    write_start();
    this->_server_ip = ip;
    write_end();
    this->write_config();
}
void nar::Global::set_server_port(unsigned short port) {
    write_start();
    this->_server_port = port;
    write_end();
    this->write_config();
}
void nar::Global::set_nar_folder(std::string& fold) {
    write_start();
    this->_nar_folder = fold;
    write_end();
    this->write_config();
}
void nar::Global::set_file_folder(std::string& fold) {
    write_start();
    this->_file_folder = fold;
    write_end();
    this->write_config();
}
void nar::Global::set_machine_id(std::string& id) {
    write_start();
    this->_machine_id = id;
    write_end();
    this->write_config();
}

void nar::Global::write_config() {
    this->_file_mtx.lock();
    std::cout << "conf file: " << this->_config_path.c_str() << std::endl;
    this->_config_file.open(this->_config_path.c_str(), std::ios::out | std::ios::trunc);
    this->_config_file.write("NAR_FOLDER=", 11);
    this->_config_file.write(this->_nar_folder.c_str(), this->_nar_folder.size());
    this->_config_file.write("\nFILE_FOLDER=", 13);
    this->_config_file.write(this->_file_folder.c_str(), this->_file_folder.size());
    this->_config_file.write("\nSERVER_IP=", 11);
    this->_config_file.write(this->_server_ip.c_str(), this->_server_ip.size());
    this->_config_file.write("\nSERVER_PORT=", 13);
    std::string port_str = std::to_string(this->_server_port);
    this->_config_file.write(std::to_string(this->_server_port).c_str(), std::to_string(this->_server_port).size());
    this->_config_file.write("\nMACHINE_ID=", 12);
    this->_config_file.write(this->_machine_id.c_str(), this->_machine_id.size());
    this->_config_file.write("\n", 1);
    this->_config_file.close();
    this->_file_mtx.unlock();
}

nar::Socket* nar::Global::establish_server_connection() {
    read_start();
    nar::Socket *serverSck = new nar::Socket(this->_ioserv,'c');
	serverSck->connect(this->get_server_ip(),this->get_server_port());
    read_end();
	return serverSck;
}
std::string nar::Global::machine_register() {

    std::cout << "Your Machine is not registered." << std::endl;
    char exs_user = 'Y';
    std::cout << "Do you have an existing user? [Y/n] ";
    std::string line;
    std::getline(std::cin, line);
    std::cout << line.size() << " " << line << std::endl;
    line = nar::trim(line);
    std::cout << line << std::endl;
    if(line.size() > 0) exs_user = std::toupper(line[0]);
    std::cout << exs_user << std::endl;
    std::string machineid;
    if(exs_user == 'Y') {
        std::cout << "todo" << std::endl;
        exit(0);
    } else {
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);
        username = nar::trim(username);
        std::cout << username << "<<<" << std::endl;
        std::string password;
        std::cout << "Password: ";
        std::getline(std::cin, password);
        password = nar::trim(password);
        std::cout << "diri diri " << std::endl;
        nar::UserVariables uservars(std::string(""), username, password);
        std::cout << "diri diri " << std::endl;
        nar::MessageTypes::IPCRegister::Request ipc_register(username, password, std::string(""));
        nar::ActiveTask::Register register_task(this, &uservars);
        std::cout << "???" << std::endl;
        register_task.run(&ipc_register);

        std::cout << "here i am " << std::endl;

        nar::Socket* macreqsock = this->establish_server_connection();
        auto tmp = nar::ActiveTask::user_authenticate(macreqsock, &uservars);


        long long int quota_default = 10000;
        std::cout << "Montly network usage(in MB) [10000MB]: " << std::endl;
        std::string quota;
        std::getline(std::cin, quota);
        std::cout << "quota:: " << quota << std::endl;
        quota = nar::trim(quota);
        if(quota.size() > 0) quota_default = std::stoll(quota);

        long long int diskspace_default = 10000;
        std::cout << "Total disk space (in MB) [10000MB]: " << std::endl;
        std::string diskspace;
        std::getline(std::cin, diskspace);
        diskspace = nar::trim(diskspace);
        if(diskspace.size() > 0) diskspace_default = std::stoll(diskspace);

        nar::MessageTypes::MachineRegister::Request macreq(quota_default, diskspace_default);
        nar::MessageTypes::MachineRegister::Response macresp;
        macreq.send_mess(macreqsock,macresp);

        machineid = macresp.get_machine_id();


    }

    return machineid;

}
