#ifndef AES_CRYPTION_H
#define AES_CRYPTION_H

#include <iostream>
#include <crypto++/aes.h>

/**
* Class which provides the interface and services for aes cryption operations
* 
* @author: utku
* @privar: _keyString, std::string, holds the aes key in the std::string form.
* @privar: _aes, byte*, holds the aes key in 'byte' type for later use with aes.h functions.
* @tested: yes
* @todo: Check todos of encyrpt and decrypt.
**/


class AesCryptor {
    public:

        /**
        * Aes Cryptor constructor with aes key as paramater.
        * @author: utku
        * @param: key, std::string, aes key in std::string type
        * @tested: yes
        * @todo: -
        **/
        AesCryptor(std::string key);

        /**
        * Static function to provide aes key generation utility.
        * @author: utku
        * @param: key, std::string&, key is generated and filled to this argument.
        * @param: length, int, length of the aes key which is to be generated.
        * @tested: yes
        * @todo: -
        **/
        static void generate_key(std::string &key, int length);

        /**
        * Encrypts the data with the aes key which is provided in the constructor. Encrpyted data is written to the 'results' paramater.
        * @author: utku
        * @param: data, std::string&, Data to be encrypted.
        * @param: result, std::string&, Variable which encrypted data will be written to.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void encrypt(std::string& data, std::string& result);

        /**
        * Decrypts the data with the aes key which is provided in the constructor. Decrpyted data is written to the 'results' paramater.
        * @author: utku
        * @param: data, std::string&, Data to be decrypted.
        * @param: result, std::string&, Variable which decrypted data will be written to.
        * @tested: yes
        * @todo: There are Magic numbers. It may be nice to get rid of them and provide a more subtle solution.
        **/
        void decrypt(std::string& data, std::string& result);
        ~AesCryptor();
    private:
        std::string _keyString;
        byte *_aes;
};

#endif
