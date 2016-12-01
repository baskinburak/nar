#ifndef NAR_FILECRYPTOR_H
#define NAR_FILECRYPTOR_H

#include <sys/types.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>

namespace nar {
    class FileCryptor {
        private:
            nar::FileKeeper* file;
            nar::FileKeeper* cryptedfile;
            std::string key;
            std::string generateUniqueFilename();
        public:
            FileCryptor(nar::FileKeeper* keep, std::string a);
            ~FileCryptor();
            int getBytes(size_t start, size_t buffersize, char* buffer);
    };
}

#endif
