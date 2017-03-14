#include <iostream>
#include <crypto++/aes.h>

class AesCryptor {
    public:
        AesCryptor(std::string key);
        static void generate_key(std::string &key, int length);
        void encrypt(std::string& data, std::string& result);
        void decrypt(std::string& data, std::string& result);
        ~AesCryptor();
    private:
        std::string _keyString;
        byte *_aes;
        std::string _text;
};
