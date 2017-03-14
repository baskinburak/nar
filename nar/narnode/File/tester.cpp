#include "File.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "base64.h"
#include <boost/filesystem.hpp>

int main() {
    /*
        usage comment

        you should create file like below code and should delete temp file after compression or decompression
    */
    boost::filesystem::path temp = boost::filesystem::unique_path();
    std::string temp_path =temp.native();
    temp_path+=std::string(".z");
    std::cout<<temp_path<<std::endl;
    nar::File testfile("./testfile.txt","r");
    nar::File dude(temp_path,"w");
    nar::File result("./final.txt","w");
    testfile.compress(dude);
    nar::File dude2(temp_path,"r");
    dude2.decompress(result);
    return 0;
}
