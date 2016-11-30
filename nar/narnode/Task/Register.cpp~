#include <nar/narnode/Task/Register.h>
#include <iostream>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/json.hpp>

using namespace nlohmann;

/*
{
    "header": {
        "action": "register",
        "channel": "ps"
    },
    "payload" : {
        "username": uname,
        "aes": aes_key
    }
}
*/
void nar::task::Register::run(int unx_sockfd, nar::Global* globals) {
    nar::Socket* conn = new nar::Socket();
    conn->create();
    conn->connect((globals->get_narServerIp()).c_str(), globals->get_narServerPort());

    //ITask::handshake(*conn, );

    json jsn;
    

    std::cout << "register " << username << std::endl;
}
