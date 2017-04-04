#include "rsa.h"
#include "aes.h"
#include <crypto++/osrng.h>
#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>

#include <nar/lib/Exception/Exception.h>

//  using namespace CryptoPP;

RsaCryptor::RsaCryptor( std::string& pub, std::string& pri) {
    set_pub(pub);
    set_pri(pri);
}

RsaCryptor::RsaCryptor() {
    return;
}

void RsaCryptor::set_pri(std::string &pri) {
    if(pri.size()) {
        CryptoPP::StringSource stringSource2(pri,true);
        _pri.BERDecode(stringSource2);
    }
}

void RsaCryptor::set_pub(std::string& pub) {
    if(pub.size()) {
        CryptoPP::StringSource stringSource(pub, true);
        _pub.BERDecode(stringSource);
    }
}


void RsaCryptor::generate_key_pair(std::string& pub, std::string& pri) {
    try {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::InvertibleRSAFunction params;
        params.GenerateRandomWithKeySize(rng, 3072);
        CryptoPP::RSA::PrivateKey privateKey(params);
        CryptoPP::RSA::PublicKey publicKey(params);               // Keys Generated up to here.

        CryptoPP::StringSink stringSink(pub);
        publicKey.DEREncode(stringSink);

        CryptoPP::StringSink stringSink2(pri);
        privateKey.DEREncode(stringSink2);
    }
    catch (...){
        throw nar::Exception::Cryption::RsaError("Error in Rsa key-generation");
    }
    return;
}

void RsaCryptor::encrypt_(std::string& data, std::string& crypted) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(_pub);
    CryptoPP::StringSource ss1(data, true,
                            new CryptoPP::PK_EncryptorFilter(rng, e, new CryptoPP::StringSink(crypted)) // PK_EncryptorFilter
                    ); // StringSource
}

void RsaCryptor::decrypt_(std::string& crypted, std::string& data) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(_pri);
    CryptoPP::StringSource ss2(crypted, true,
                              new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(data)) // PK_DecryptorFilter
                    ); // StringSource
}


void RsaCryptor::encrypt(std::string& data, std::string& crypted) {
    try {
        if(data.length() <= 342) {
            encrypt_(data,crypted);
            std::cout << "FAST CONVERSION" << crypted.length() << std::endl;
            return;
        }

        std::string aesStr;

        AesCryptor::generate_key(aesStr,16);
        AesCryptor aes(aesStr);
        aes.encrypt(data,crypted);

        std::string cryStr;
        encrypt_(aesStr,cryStr);
        crypted.insert(0,cryStr);
    }
    catch (...){
        throw nar::Exception::Cryption::RsaError("Error in Rsa encryption");
    }
    return;
}
void RsaCryptor::decrypt(std::string& crypted, std::string& data) {
    try {
        if(crypted.length()<=384) {
            decrypt_(crypted,data);
            return;
        }

        std::string cryStr(crypted.substr(0,384)),aesStr;
        decrypt_(cryStr, aesStr);

        AesCryptor aes(aesStr);
        std::string keyless_data(crypted.substr(384,crypted.length()-384));
        aes.decrypt(keyless_data,data);
    }
    catch (...) {
        throw nar::Exception::Cryption::RsaError("Error in Rsa decryption");
    }
    return;
}
