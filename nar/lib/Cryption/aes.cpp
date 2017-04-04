#include <string>
#include <string.h>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>

#include <nar/lib/Exception/Exception.h>

#include "aes.h"

void byte_to_string( byte* data, std::string& str, int len) {
    for(int i=0; i<len; i++) {
        str.push_back(data[i]);
    }
    return;
}

byte* string_to_byte(std::string& key) {
    int len = key.length();
    byte *aes = new byte[len];
    for(int i=0; i<len; i++) {
        aes[i] = key[i];
    }
    return aes;
}

AesCryptor::AesCryptor(std::string key): _keyString(key) {
    int keyLen = _keyString.length();
    const char *str = _keyString.c_str();
    _aes = string_to_byte(_keyString);
}


AesCryptor::~AesCryptor() {
    free(_aes);
}




void AesCryptor::encrypt(std::string &text, std::string &crypted) {
    try {
        byte iv[256];
        const int TAG_SIZE = 12;

        CryptoPP::AutoSeededRandomPool pool;
        pool.GenerateBlock(iv, 256);

        CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
        enc.SetKeyWithIV(_aes, 16, iv, 256);

        CryptoPP::StringSource ss1(text, true, new CryptoPP::AuthenticatedEncryptionFilter(enc, new CryptoPP::StringSink(crypted), false, TAG_SIZE));

        //byte_to_string(cipher,text.length());
        std::string bla;
        byte_to_string(iv,bla,256);


        std::string test = crypted;
        crypted = bla + crypted;
    }
    catch (...){
        throw nar::Exception::Cryption::AesError("Error in Aes encryption");
    }
    return;
}

void AesCryptor::decrypt(std::string& data, std::string& result) {
    try {
        std::string ivStr = data.substr(0,256);
        byte* iv = string_to_byte(ivStr);
        CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(_aes, 16, iv, 256);

        const int TAG_SIZE = 12;

        CryptoPP::AuthenticatedDecryptionFilter df(dec, new CryptoPP::StringSink(result), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, TAG_SIZE);

        CryptoPP::StringSource ss(data.substr(256), true, new CryptoPP::Redirector(df));
        free(iv);
    }
    catch (...){
        throw nar::Exception::Cryption::AesError("Error in Aes decryption");
    }
    return;
}

void AesCryptor::generate_key(std::string &key, int length) {
    try {
        std::string chars(
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "1234567890"
            "!@#$%^&*()"
            "`~-_=+[{]}\\|;:'\",<.>/? ");
        boost::random::random_device rng;
        boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
        for(int i = 0; i < length; ++i) {
            key.push_back(chars[index_dist(rng)]);
        }
    }
    catch (...){
        throw nar::Exception::Cryption::AesError("Error in Aes key-generation");
    }
    return;
}
