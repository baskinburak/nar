#include "File.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "base64.h"
#include <boost/filesystem.hpp>


std::string aes("abcdabcdabcdabcd");


void encTest1() {
    nar::File testfile("./testfile.txt","r",false);
    nar::File *temp = testfile.encrypt(aes);
    std::string path("./son.txt");
    nar::File *temp2 = temp->decrypt(aes,path);
     
}


int main() {

    /*nar::File testfile("./testfile.txt","r",false);
    nar::File* temp = testfile.compress();
    nar::File* temp2 = temp->decompress();
    delete temp;
    delete temp2;*/

    encTest1();

    return 0;
}
