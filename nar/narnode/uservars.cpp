#include "uservars.h"
#include <iostream>
#include <crypto++/sha.h>
#include <crypto++/hex.h>
#include <crypto++/pwdbased.h>
using std::cout;
using std::endl;
using std::string;
using namespace CryptoPP;
nar::UserVariables::UserVariables(std::string current_directory, std::string username, std::string password): _current_directory(current_directory), _username(username) {

// From https://www.ietf.org/rfc/rfc6070.txt
//   PKCS #5: Password-Based Key Derivation Function 2 (PBKDF2) Test Vectors
//
//      Input:
//       P = "password" (8 octets)
//       S = "salt" (4 octets)
//       c = 1
//       dkLen = 20
//
//     Output:
//       DK = 0c 60 c8 0f 96 1f 0e 71
//            f3 a9 b5 24 af 60 12 06
//            2f e0 37 a6    (20 octets)

    byte *password_b = new byte[password.size()];
    for(int i= 0;i<password.size();i++) {
        password_b[i] = (byte)password[i];
    }

    size_t plen = strlen((const char*)password_b);

    byte salt[] = "qweqqwe";
    size_t slen = strlen((const char*)salt);

    int c = 1;
    byte derived[8];

    PKCS5_PBKDF2_HMAC < CryptoPP::SHA1 > pbkdf2;
    pbkdf2.DeriveKey(derived, sizeof(derived), 0, password_b, plen, salt, slen, c);

    string result;
    HexEncoder encoder(new StringSink(result));

    encoder.Put(derived, sizeof(derived));
    encoder.MessageEnd();

    cout << "Derived: " << result << endl;
    _pass_aes =  result;

}

std::string& nar::UserVariables::get_current_directory() {
    return this->_current_directory;
}

std::string& nar::UserVariables::get_username() {
    return this->_username;
}

std::string& nar::UserVariables::get_pass_aes() {
    return this->_pass_aes;
}
