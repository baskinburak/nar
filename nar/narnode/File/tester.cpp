#include "File.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "base64.h"
#include <boost/filesystem.hpp>

int main() {

    nar::File testfile("./testfile.txt","r",false);
    nar::File* temp = testfile.compress();
    nar::File* temp2 = temp->decompress();
    delete temp;
    delete temp2;

    return 0;
}
