#ifndef NAR_FILEDECRYPTOR_H
#define NAR_FILEDECRYPTOR_H
#include <nar/narnode/FileKeeper/FileKeeper.h>

namespace nar {
    class FileDecryptor {
        private:
            std::string data;
            std::string key;
        public:
            FileDecryptor(std::string d, std::string aes): data(d), key(aes) {}
            nar::FileKeeper* decrypt(std::string filename);
    };
}

#endif
