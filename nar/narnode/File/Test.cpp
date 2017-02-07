#include <iostream>
#include <nar/narnode/File/File.h>
#include <nar/lib/Exception/Exception.h>
#include <cstring>
int main(int argc, char* argv[]) {
    nar::File file(argv[1], "r");
    try {
        int size = file.size();
        char buf[1025];
        for(int i=0; i<size; i+=1024) {
            int total = file.read(buf, i, 1024);
            std::string cur(buf, total);
            std::cout << cur;
        }
    } catch(nar::Exception::ExcpBase& exp) {
        std::cout << exp.what() << std::endl;
    }

    nar::File file2(argv[2], "w");
    char* buf = "selam arkadaslar <3";
    file2.write(buf, strlen(buf));
    return 0;
}
