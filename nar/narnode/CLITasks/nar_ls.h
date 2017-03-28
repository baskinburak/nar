#ifndef NAR_CLI_TASKS_LS_H
#define NAR_CLI_TASKS_LS_H

#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/lib/Socket/Socket.h>

#include <iostream>
#include <string>
#include <vector>

namespace nar{
    namespace CLITasks{
        void nar_ls(std::string dir_name){
            MessageTypes::IPCLs::Request req(dir_name);

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
