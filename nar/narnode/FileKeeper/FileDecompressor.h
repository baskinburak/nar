#ifndef NAR_FILEDECOMPRESSOR_H
#define NAR_FILEDECOMPRESSOR_H
#include <nar/narnode/FileKeeper/FileKeeper.h>

namespace nar {
    class FileDecompressor {
        private:
            std::string data;
        public:
            FileDecompressor(std::string d): data(d) {}
            nar::FileKeeper* decompress(std::string filename);
    };
}

#endif
