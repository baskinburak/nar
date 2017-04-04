#ifndef RSA_CRYPTION_H
#define RSA_CRYPTION_H
#include <crypto++/rsa.h>
#include <string>



/**
* Class which provides the interface and services for RSA encrpytion and decryption operations.
*
* @author: utku
* @privar: _pri, CryptoPP::RSA::PrivateKey, stores the private key in cyrpto++ container
* @privar: _pub, CryptoPP::RSA::PublicKey, stores the public key in cyrpto++ container
* @tested: yes
* @todo: -
**/

class RsaCryptor {
    public:

        /**
        * Rsa Cryptor constructor with rsa keys as paramater.
        * @author: utku
        * @param: pub, std::string&, rsa public key in std::string type
        * @param: pri, std::string&, rsa private key in std::string type
        * @tested: yes
        * @todo: -
        **/
        RsaCryptor(std::string &pub, std::string &pri);                       // Parameters are optional
        RsaCryptor();

        /**
        * Setters for private and public rsa keys.
        * @author: utku
        * @param: key, std::string&, rsa public key in std::string type
        * @tested: yes
        * @todo: -
        **/
        void set_pri(std::string& key);
        void set_pub(std::string& key);

        /**
        * Static function to provide rsa key generation utility. Generated keys are written to the paramaters.
        * @author: utku
        * @param: pub, std::string&, Generated public rsa key is written to this argument.
        * @param: pri, std::string&, Generated private rsa key is written to this argument.
        * @throws: nar::Exception::Cryption::RsaError, if key generation fails.
        * @tested: yes
        * @todo: -
        **/
        static void generate_key_pair(std::string& pub, std::string& pri);

        /**
        * Encrypts the data provided by 'data' argument into the 'crypted' argument. For success, public key must be either given in constructor or set later via setter.
        * @author: utku
        * @param: data, std::string&, Data to be crypted.
        * @param: crypted, std::string&, Crypted data is written here.
        * @throws: nar::Exception::Cryption::RsaError, if encryption fails or pub key is not provided before.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void encrypt(std::string& data, std::string& crypted);

        /**
        * Decrypts the data provided by 'crypted' argument into the 'data' argument. For success, private key must be either given in constructor or set later via setter.
        * @author: utku
        * @param: crypted, std::string&, Data to be decrypted.
        * @param: data, std::string&, Decrypted data is written here.
        * @throws: nar::Exception::Cryption::RsaError, if decryption fails or pri key is not provided before.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void decrypt(std::string& crypted, std::string& data);


    private:

       /**
        * Encrypts the data provided by 'data' argument into the 'crypted' argument. For success, public key must be either given in constructor or set later via setter.
        * This function differs from its public version. There are limits to the size of the data that can be feed into this. Other version calls this version.
        * @author: utku
        * @param: data, std::string&, Data to be crypted.
        * @param: crypted, std::string&, Crypted data is written here.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void encrypt_(std::string& data, std::string& crypted);

        /**
        * Decrypts the data provided by 'crypted' argument into the 'data' argument. For success, private key must be either given in constructor or set later via setter.
        * This function differs from its public version. There are limits to the size of the data that can be feed into this. Other version calls this version.
        * @author: utku
        * @param: crypted, std::string&, Data to be decrypted.
        * @param: data, std::string&, Decrypted data is written here.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void decrypt_(std::string& crypted, std::string& data);

        CryptoPP::RSA::PrivateKey _pri;
        CryptoPP::RSA::PublicKey _pub;
};

#endif
