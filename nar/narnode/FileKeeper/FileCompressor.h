#ifndef NAR_FILECOMPRESSOR_H
#define NAR_FILECOMPRESSOR_H

#include <zlib.h>
#include <stdio.h>

#include <nar/narnode/FileKeeper/FileKeeper.h>

namespace nar {
    class FileCompressor {
        private:
            nar::FileKeeper* file;
            nar::FileKeeper* compressedfile;
        public:
            FileCompressor(nar::FileKeeper *myfile) : file(myfile) {}
            unsigned long file_size(char *filename);
            int decompress_one_file(nar::FileKeeper *infile, nar::FileKeeper *outfile);
            int compress_one_file(nar::FileKeeper *infile, nar::FileKeeper *outfile);
    };
}

#endif
