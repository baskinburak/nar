#include <nar/narnode/FileKeeper/FileDecryptor.h>
#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <nar/narnode/utility.h>
#include <cstdio>



nar::FileKeeper* nar::FileDecryptor::decrypt(std::string filename) {
    nar::FileKeeper* file = new nar::FileKeeper(filename);
    std::cout << key << std::endl;
    byte* aes = nar::hex_to_byte(key); // 16 bytes

    std::string ivs = data.substr(0, 512);
    byte* iv = hex_to_byte(ivs); // 256 bytes

    std::cout << ivs << std::endl;


    data = data.substr(512);


    CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(aes, 16, iv, 256);

    const int TAG_SIZE = 12;
    std::string plaintext;

    CryptoPP::AuthenticatedDecryptionFilter df(dec, new CryptoPP::StringSink(plaintext), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, TAG_SIZE);

    CryptoPP::StringSource ss(data, true, new CryptoPP::Redirector(df));


    if(df.GetLastResult()) {
        file->writeToFile(file->getFd(), plaintext.size(), plaintext.c_str());
        return file;
    } else {
        delete file;
        return NULL;
    }
}
