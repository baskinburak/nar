#include "../nar/narnode/FileKeeper/FileKeeper.h"
#include "../nar/narnode/FileKeeper/FileCryptor.h"
#include "../nar/narnode/FileKeeper/FileDecryptor.h"
#include <cstdlib>
#include <ctime>
#include <crypto++/filters.h>
#include <fstream>
#include <streambuf>
#include <string>
#include <iterator>
#include <iostream>

int main() {
    srand(time(NULL));
    std::ifstream t("/tmp/dbfkblfjnqabftnrsdzglrjkzdjynxns", std::ios::binary);
    t.unsetf(std::ios_base::skipws);

  /*  nar::FileKeeper keep(std::string("/home/bbsen/nar/nar/narnode/FileKeeper/FileKeeper.h"));
    nar::FileCryptor crypt(&keep, std::string("11d6958f37161a4ca21a15ae19d566a4"));*/

    std::string data((std::istream_iterator<char>(t)), (std::istream_iterator<char>()));
    std::cout << data << std::endl;
    std::cout << "real len: " << data.size() << std::endl;
    nar::FileDecryptor dec(data, std::string("11d6958f37161a4ca21a15ae19d566a4"));
    nar::FileKeeper* keep = dec.decrypt(std::string("/tmp/decrypted"));
    if(keep == NULL) {
        std::cout << "decrypt failed" << std::endl;
    } 

    delete keep;
    return 0;
}
