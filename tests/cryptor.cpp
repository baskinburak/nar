#include "../nar/narnode/FileKeeper/FileKeeper.h"
#include "../nar/narnode/FileKeeper/FileCryptor.h"
#include <cstdlib>
#include <ctime>
int main() {
    srand(time(NULL));
    nar::FileKeeper* keep = new nar::FileKeeper(std::string("/home/bbsen/nar/tests/aes.cpp"));
    char arr[128];
    keep->getBytes(0,128,arr);
    std::cout << arr << std::endl;
    nar::FileCryptor(keep, std::string("12345"));
    return 0;
}
