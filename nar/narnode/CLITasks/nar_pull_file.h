#ifndef NAR_CLI_TASKS_PULL_H
#define NAR_CLI_TASKS_PULLS_H

#include "../../lib/Messaging/MessageTypes/IPCPull.h"
#include "../../lib/Socket/Socket.h"

#include <iostream>
#include <string>
#include <vector>

namespace nar{
    namespace CLITasks{
        void nar_pull_file(std::string file_name){
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
            cli_skt.connect(std::string("127.0.0.1"), 23456);

            req.send_action(&cli_skt);
            req.print_loop(&cli_skt);
            return;
        }
    }
}

#endif
