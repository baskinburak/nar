#include "../nar/narnode/FileKeeper/FileKeeper.h"
#include "../nar/narnode/FileKeeper/FileCompressor.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <streambuf>
#include <string>
#include <iterator>
#include <iostream>

int main() 
{    
    nar::FileKeeper fk(std::string("/home/doguhan/Desktop/bitirme/nar/nar/narnode/FileKeeper/FileKeeper.h"));
    //nar::FileKeeper fk1(std::string("/home/doguhan/Desktop/comTest/abc.txt"));
    
    char cptr[] = "/home/doguhan/Desktop/bitirme/nar/nar/narnode/FileKeeper/FileKeeper.h";
    char cptr2[] = "/home/doguhan/Desktop/comTest/abc.txt";
    char cptr3[] = "/home/doguhan/Desktop/comTest/das.h";
    
    nar::FileCompressor fc(&fk);
    fc.compress_one_file(cptr, cptr2);
    fc.decompress_one_file(cptr2,cptr3);
    

    return 0;
}