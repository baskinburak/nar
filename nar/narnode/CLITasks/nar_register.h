#ifndef NAR_CLI_TASKS_REGISTER_H
#define NAR_CLI_TASKS_REGISTER_H

#include <nar/lib/Messaging/MessageTypes/IPCRegister.h>
#include <nar/lib/Socket/Socket.h>

#include <iostream>
#include <string>
#include <vector>

namespace nar{
    namespace CLITasks{
        void nar_register(std::string user_name){
            MessageTypes::IPCRegister::Request req(user_name);

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
