#ifndef NAR_FILECOMPRESSOR_H
#define NAR_FILECOMPRESSOR_H

#include <zlib.h>
#include <stdio.h>

#include "FileKeeper.h"

namespace nar {
    class FileCompressor {
        public:
            FileCompressor()=default;
            unsigned long file_size(std::string fileName);
            int decompress_one_file(std::string inFilename, std::string outFilename);
            int decompress_one_file(FileKeeper *in, FileKeeper *out);
            int compress_one_file(std::string inFilename, std::string outFilename);
            int compress_one_file(FileKeeper *in, FileKeeper *out);
    };
}

#endif

