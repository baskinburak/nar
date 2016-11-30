#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <iostream>
int main() {
    byte key[CryptoPP::AES::MAX_KEYLENGTH];
    byte iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

    std::string plaintext("123456781234567812345678123456781234567812345678");
    std::string crypted;

    CryptoPP::AES::Encryption aesEnc(key, CryptoPP::AES::MAX_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEnc(aesEnc, iv);

    CryptoPP::StreamTransformationFilter stfEnc(cbcEnc, new CryptoPP::StringSink(crypted));
    stfEnc.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()),plaintext.size());
    stfEnc.MessageEnd();

    std::cout << crypted;

    CryptoPP::AES::Decryption aesDec(key, CryptoPP::AES::MAX_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDec(aesDec, iv);

    std::string decrypted;

    CryptoPP::StreamTransformationFilter stfDec(cbcDec, new CryptoPP::StringSink(decrypted));
    stfDec.Put(reinterpret_cast<const unsigned char*>(crypted.c_str()), crypted.size());
    stfDec.MessageEnd();

  //  std::cout << crypted.size() << std::endl;

   // std::cout << decrypted << std::endl;
   // std::cout << decrypted.size() << std::endl;

    return 0;
}
