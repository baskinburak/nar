#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <stdio.h>
#include <iostream>
int main() {
    std::cout << CryptoPP::AES::DEFAULT_KEYLENGTH << std::endl;
    std::cout << CryptoPP::AES::BLOCKSIZE << std::endl;

    CryptoPP::AutoSeededRandomPool pool;

    byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    pool.GenerateBlock(key, sizeof(key));
    for(int i=0; i<sizeof(key); i++)
        printf("\\x%02X", key[i]);
    std::cout << std::endl;
    byte iv[CryptoPP::AES::BLOCKSIZE * 16];
    pool.GenerateBlock(iv, sizeof(iv));

    const int TAG_SIZE = 12;

    std::string pdata("here some data pls");
    std::string cipher, encoded;

    CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(key, sizeof(key), iv, sizeof(iv));

    CryptoPP::StringSource ss1(pdata, true, new CryptoPP::AuthenticatedEncryptionFilter(enc, new CryptoPP::StringSink(cipher), false, TAG_SIZE));

    std::cout << cipher;
  //  std::cout << crypted.size() << std::endl;

   // std::cout << decrypted << std::endl;
   // std::cout << decrypted.size() << std::endl;

    return 0;
}
