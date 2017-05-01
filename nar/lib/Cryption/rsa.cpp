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
        try {
            _pri.BERDecode(stringSource2);
        }
        catch( CryptoPP::Exception& e ) {
            throw nar::Exception::Cryption::RsaError(std::string("Rsa::set_pri ").append(e.what()) );
        }
    }
}

void RsaCryptor::set_pub(std::string& pub) {
    if(pub.size()) {
        CryptoPP::StringSource stringSource(pub, true);
        try {
            _pub.BERDecode(stringSource);
        }
        catch( CryptoPP::Exception& e ) {
            throw nar::Exception::Cryption::RsaError(std::string("Rsa::set_pub ").append(e.what()) );
        }
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
    catch (CryptoPP::Exception& e ){
        throw nar::Exception::Cryption::RsaError(std::string("Rsa::generate_key ").append(e.what()) );
    }
    return;
}

void RsaCryptor::encrypt_(std::string& data, std::string& crypted) {
    try {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::RSAES_OAEP_SHA_Encryptor e(_pub);
        CryptoPP::StringSource ss1(data, true,
                                new CryptoPP::PK_EncryptorFilter(rng, e, new CryptoPP::StringSink(crypted)) // PK_EncryptorFilter
                        ); // StringSource
    }
    catch ( CryptoPP::Exception& e ) {
        throw nar::Exception::Cryption::RsaError(std::string("Rsa::encrypt ").append(e.what()) );
    }
}

void RsaCryptor::decrypt_(std::string& crypted, std::string& data) {
    try {
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::RSAES_OAEP_SHA_Decryptor d(_pri);
        CryptoPP::StringSource ss2(crypted, true,
                                  new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(data)) // PK_DecryptorFilter
                        ); // StringSource
    }
    catch (CryptoPP::Exception& e ) {
        throw nar::Exception::Cryption::RsaError(std::string("Rsa::decrpyt ").append(e.what()) );
    }
}


void RsaCryptor::encrypt(std::string& data, std::string& crypted) {

        if(data.length() <= 342) {
            encrypt_(data,crypted);
            std::cout << "FAST CONVERSION" << crypted.length() << std::endl;
            return;
        }
    try {
        std::string aesStr;

        AesCryptor::generate_key(aesStr,16);
        AesCryptor aes(aesStr);
        aes.encrypt(data,crypted);

        std::string cryStr;
        encrypt_(aesStr,cryStr);
        crypted.insert(0,cryStr);
    }
    catch (CryptoPP::Exception& e){
        throw nar::Exception::Cryption::RsaError(std::string("Rsa::encrypt ").append(e.what()) );
    }
    return;
}
void RsaCryptor::decrypt(std::string& crypted, std::string& data) {
        if(crypted.length()<=384) {
            decrypt_(crypted,data);
            return;
        }
    try {
        std::string cryStr(crypted.substr(0,384)),aesStr;
        decrypt_(cryStr, aesStr);

        AesCryptor aes(aesStr);
        std::string keyless_data(crypted.substr(384,crypted.length()-384));
        aes.decrypt(keyless_data,data);
    }
    catch (CryptoPP::Exception& e) {
        throw nar::Exception::Cryption::RsaError(std::string("Rsa::decrypt").append(e.what()) );
    }
    return;
}
