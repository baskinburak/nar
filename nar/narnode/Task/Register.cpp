#include <nar/narnode/Task/Register.h>
#include <iostream>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/json.hpp>
#include <crypto++/filters.h>
#include <crypto++/aes.h>
#include <crypto++/osrng.h>
#include <nar/narnode/utility.h>

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

    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    CryptoPP::AutoSeededRandomPool pool;
    pool.GenerateBlock(key, sizeof(key));
    
    std::string hex = nar::byte_to_hex(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    std::cout << hex << " " << hex.size() << std::endl;

    json jsn;
    
    jsn["header"]["action"] = "register";
    jsn["header"]["channel"] = "ps";
    jsn["payload"]["username"] = username;
    jsn["payload"]["aes"] = hex;

    nar::send_message(*conn, jsn.dump());

    auto response = json::parse(nar::get_message(*conn));
    std::cout << response.dump() << std::endl;
    if(response["header"]["status-code"] == 200) {
        nar::send_ipc_message(unx_sockfd, std::string("Register successful"));
    } else {
        nar::send_ipc_message(unx_sockfd, std::string("ERROR ") + std::to_string((int)response["header"]["status-code"]));
    }

    nar::send_ipc_message(unx_sockfd, std::string("END"));
	globals->set_username(username);	
    std::string conf = globals->get_configFolder();
}
