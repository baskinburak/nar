#ifndef NAR_CLI_TASKS_PUSH_H
#define NAR_CLI_TASKS_PUSH_H

#include "../../lib/Messaging/MessageTypes/IPCPush.h"
#include "../../lib/Socket/Socket.h"

#include <iostream>
#include <string>
#include <vector>

namespace nar{
    namespace CLITasks{
        void nar_push_file(std::string file_name){
            char* real_path = realpath(file_name.c_str(), NULL);
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
            return;
        }
    }
}

#endif
