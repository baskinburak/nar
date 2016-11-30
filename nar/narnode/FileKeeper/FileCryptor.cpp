#include <nar/narnode/FileKeeper/FileCryptor.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

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
}

nar::FileCryptor::FileCryptor(FileKeeper* keep, std::string a) : file(keep), key(a) {
    std::string filename = generateUniqueFilename();
    cryptedfile = new FileKeeper(filename);
    
}

int nar::FileCryptor::getBytes(size_t start, size_t len, char* buf) {
    
}
