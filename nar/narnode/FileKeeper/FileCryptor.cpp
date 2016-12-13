#include <nar/narnode/FileKeeper/FileCryptor.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <nar/narnode/utility.h>
#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>


std::string nar::FileCryptor::generateUniqueFilename() {
    int NAME_LEN = 32;
    std::string filename("/tmp/");
    filename.resize(NAME_LEN+5);
    char pool[] = "abcdefghijklmnopqrstuvyzwx";
    do {
        for(int i=0; i<NAME_LEN; i++) {
            filename[i+5] = pool[rand()%26];
        }
    } while(access(filename.c_str(), F_OK) != -1);
    return filename;    
}

nar::FileCryptor::~FileCryptor() {
    free(cryptedfile);
}

nar::FileCryptor::FileCryptor(FileKeeper* keep, std::string a) : file(keep), key(a) {
    std::string filename = generateUniqueFilename();
    cryptedfile = new FileKeeper(filename);
    int size = keep->getFileSize();
    char* buff = (char*) malloc(sizeof(char)* size);
    keep->getBytes(0, size, buff);
    std::string inp(buff, buff+size);

    byte* aes = nar::hex_to_byte(key);
    byte iv[256];
    const int TAG_SIZE = 12;

    CryptoPP::AutoSeededRandomPool pool;
    pool.GenerateBlock(iv, 256);

    std::string cipher;

    CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(aes, 16, iv, 256);

    CryptoPP::StringSource ss1(inp, true, new CryptoPP::AuthenticatedEncryptionFilter(enc, new CryptoPP::StringSink(cipher), false, TAG_SIZE));

    cipher = byte_to_hex(iv, 256) + cipher;

    cryptedfile->writeToFile(cryptedfile->getFd(), cipher.size(), cipher.c_str());
    free(aes);
}

int nar::FileCryptor::getBytes(size_t start, size_t len, char* buf) {
    cryptedfile->getBytes(start, len, buf);
}
unsigned long nar::FileCryptor::getFileSize(){
	cryptedfile->getFileSize();
}

