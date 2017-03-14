#include <crypto++/rsa.h>
#include <string>

class RsaCryptor {
    public:
        RsaCryptor(std::string &pub, std::string &pri);                       // Parameters are optional
        RsaCryptor();
        void set_pri(std::string& key);
        void set_pub(std::string& key);
        static void generate_key_pair(std::string& pub, std::string& pri);

        void encrypt(std::string& data, std::string& crypted);
        void decrypt(std::string& crypted, std::string& data);
    

    private:
        void encrypt_(std::string& data, std::string& crypted);
        void decrypt_(std::string& crypted, std::string& data);

        CryptoPP::RSA::PrivateKey _pri;
        CryptoPP::RSA::PublicKey _pub;
};
