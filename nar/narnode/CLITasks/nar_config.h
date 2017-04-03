#ifndef NAR_CLI_TASKS_CONFIG_H
#define NAR_CLI_TASKS_CONFIG_H

#include "../../lib/Messaging/MessageTypes/IPCConfig.h"
#include "../../lib/Socket/Socket.h"

#include <iostream>
#include <string>
#include <vector>

namespace nar{
    namespace CLITasks{
        void nar_config(std::string var, std::string value){
            MessageTypes::IPCConfig::Request req(var, value);

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
