#include <nar/narnode/global.h>
#include <iostream>

nar::Global::Global(std::string& config_path): _config_path(config_path) {
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

        for(; i<line.size(); i++)
            value.push_back(line[i]);

        if(key == "NAR_FOLDER") {
            mask |= 1;
            this->_nar_folder = value;
        } else if(key == "FILE_FOLDER") {
            mask |= 2;
            this->_file_folder = value;
        } else if(key == "MACHINE_ID") {
            mask |= 4;
            this->_machine_id = value;
        } else if(key == "SERVER_IP") {
            mask |= 8;
            this->_server_ip = value;
        } else if(key == "SERVER_PORT") {
            mask |= 16;
            this->_server_port = std::stoi(value);
        }
    }

    if(mask != (1 | 2 | 4 | 8 | 16)) {
        std::cout << "Config file is not complete" << std::endl;
        exit(0);
    }
}

nar::Global::~Global() {
    this->write_config();
    this->_config_file.close();
}

std::atomic<std::string>& nar::Global::get_server_ip() {
    return this->_server_ip;
}
std::atomic<unsigned short>& nar::Global::get_server_port() {
    return this->_server_port;
}
std::atomic<std::string>& nar::Global::get_machine_id() {
    return this->_machine_id;
}
std::atomic<std::string>& nar::Global::get_nar_folder() {
    return this->_nar_folder;
}
std::atomic<std::string>& nar::Global::get_file_folder() {
    return this->_file_folder;
}
std::atomic<boost::asio::io_service>& nar::Global::get_ioserv() {
    return this->_ioserv;
}
void nar::Global::set_server_ip(const std::string& ip) {
    this->_server_ip = ip;
    this->write_config();
}
void nar::Global::set_server_port(unsigned short port) {
    this->_server_port = port;
}
void nar::Global::set_nar_folder(std::string& fold) {
    this->_nar_folder = fold;
}
void nar::Global::set_file_folder(std::string& fold) {
    this->_file_folder = fold;
}
void nar::Global::set_machine_id(std::string& id) {
    this->_machine_id = id;
}

void nar::Global::write_config() {
    this->_file_mtx.lock();
    this->_config_file.close();
    this->_config_file.open(this->config_path.c_str(), std::ios::out | std::ios::trunc);
    this->_config_file.write("NAR_FOLDER=", 11);
    this->_config_file.write(this->_nar_folder.c_str(), this->_nar_folder.size());
    this->_config_file.write("\nFILE_FOLDER=", 13);
    this->_config_file.write(this->_file_folder.c_str(), this->_file_folder.size());
    this->_config_file.write("\nMACHINE_ID=", 12);
    this->_config_file.write(this->_machine_id.c_str(), this->_machine_id.size());
    this->_config_file.write("\nSERVER_IP=", 11);
    this->_config_file.write(this->_server_ip.c_str(), this->_server_ip.size());
    this->_config_file.write("\nSERVER_PORT=\n", 14);
    std::string port_str = std::to_string(this->_server_port);
    this->_config_file.write(this->port_str.c_str(), this->port_str.size());
    this->_file_mtx.unlock();
}

nar::Socket* nar::Global::establish_server_connection() {
    nar::Socket *serverSck = new nar::Socket(this->io_serv,'c');
	serverSck->connect(this->get_narServerIp(),this->get_narServerPort());
	return serverSck;
}

