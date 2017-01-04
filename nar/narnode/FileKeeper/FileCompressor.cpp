#include <nar/narnode/FileKeeper/FileCompressor.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <nar/narnode/utility.h>

nar::FileCompressor::FileCompressor(FileKeeper* keep) : file(keep) {

}

nar::FileCompressor::~FileCompressor() {
    free(compressedfile);
}

unsigned long nar::FileCompressor::getFileSize(){
	compressedfile->getFileSize();
}
