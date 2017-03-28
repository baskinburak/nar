#include <nar/narnode/Task/Register.h>
#include <iostream>
#include <nar/lib/Socket/Socket.h>
#include <crypto++/filters.h>
#include <crypto++/aes.h>
#include <crypto++/osrng.h>
#include <nar/narnode/utility.h>


void nar::task::Register::run(nar::Socket * skt,  nar::Global* globals) {

    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    CryptoPP::AutoSeededRandomPool pool;
    pool.GenerateBlock(key, sizeof(key));

    std::string hex = nar::byte_to_hex(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    std::cout << hex << " " << hex.size() << std::endl;
    nar::MessageTypes::UserRegister::Request usr_req(user_name, hex);
    nar::MessageTypes::UserRegister::Response usr_resp(999);
    // srv_skt will be implemented;
    usr_req.send_mess(srv_skt,usr_resp);

	globals->set_username(username);
}
