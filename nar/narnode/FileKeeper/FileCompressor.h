#ifndef NAR_FILECOMPRESSOR_H
#define NAR_FILECOMPRESSOR_H

#include <nar/narnode/FileKeeper/FileKeeper.h>

namespace nar {
    class FileCompressor {
        private:
            nar::FileKeeper* file;
            nar::FileKeeper* compressedfile;
        public:
            FileCompressor(nar::FileKeeper* keep);
            ~FileCompressor();
			unsigned long getFileSize();
    };
}

#endif
