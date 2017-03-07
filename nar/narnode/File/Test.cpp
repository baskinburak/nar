#include <iostream>
#include <nar/narnode/File/File.h>
#include <nar/lib/Exception/Exception.h>
#include <cstring>
void success(const char* mess) {
    std::cout << "\033[0;32m" << mess << "\033[0m" << std::endl;
}

void fail(const char* mess) {
    std::cout << "\033[0;31m" << mess << "\033[0m" << std::endl;
}

void clean() {
    remove("/tmp/file_deneme");
}

int main() {
    char buffer[5000];
    char buffer2[5000];
    clean();
    {
        std::cout << "1 Trying reading from non-existent file with std::string constructor ";
        std::string fp("/tmp/file_deneme");
        nar::File file(fp, "r");
        try {
            file.read(buffer, 0, 1024);
            fail("FAILED");
        } catch(nar::Exception::File::NotOpen& exp) {
            success("SUCCESS");
        }
    }

    clean();
    {
        std::cout << "2 Trying reading from non-existent file with const char* constructor ";
        nar::File file("/tmp/file_deneme", "r");
        try {
            file.read(buffer, 0, 1024);
            fail("FAILED");
        } catch(nar::Exception::File::NotOpen& exp) {
            success("SUCCESS");
        }
    }

    clean();
    {
        std::cout << "3 Trying writing to a file, which does not exist initially ";
        nar::File file("/tmp/file_deneme", "w");
        char* mess = "hue sup dude";
        file.write(mess, strlen(mess));
        file.close();
        nar::File rfile("/tmp/file_deneme", "r");

        unsigned long size = rfile.size();
        if(size != strlen(mess)) {
            std::cout << "strlen ";
            fail("FAILED");
        }

        int cnt = rfile.read(buffer, 0, size);
        if(cnt != size) {
            std::cout << "read size ";
            fail("FAILED");
        }
        int i;
        for(i=0; i<cnt; i++) {
            if(mess[i] != buffer[i]) {
                std::cout << "mess mismatch ";
                fail("FAILED");
                break;
            }
        }
        if(i==cnt) {
            success("SUCCESS");
        }
    }

    clean();
    {
        std::cout << "4 Trying to open a file with unrecognized file mode ";
        try {
            nar::File file("/tmp/file_deneme", "a");
            fail("FAILURE");
        } catch(nar::Exception::File::UnrecognizedFileMode& Exp) {
            success("SUCCESS");
        }
    }

    clean();
    {
        std::cout << "5 Trying to open a file with no access rights ";
        nar::File file("/proc/1/root", "r");
        try {
            file.read(buffer, 0, 1024);
            fail("FAILURE");
        } catch(nar::Exception::File::NotOpen& Exp) {
            success("SUCCESS");
        }
    }

    clean();
    {
        std::cout << "6 Trying reading string 1000000 times with 1024 bytes ";
        nar::File file("/bin/bash", "r");
        int i;
        for(i=0; i<1000000; i++) {
            int size = file.read(buffer, 0, 1024);
            if(size != 1024) {
                fail("FAILURE");
                break;
            }
        }
        if(i==1000000)
            success("SUCCESS");
    }

    clean();
    {
        std::cout << "7 Trying to read whole file with 123 byte chunks, writing it to another file ";
        nar::File file("/bin/bash", "r");
        nar::File outfile("/tmp/file_deneme", "w");
        int size = file.size();
        int read = 0;
        for(int i=0; i<size; i+=123) {
            read = file.read(buffer, i, 123);
            outfile.write(buffer, read);
        };

        outfile.close();

        nar::File out2file("/tmp/file_deneme", "r");

        if(file.size() != out2file.size()) {
            fail("FAILURE");
        } else {
            int read = 0;
            int size = file.size();
            int i;
            for(i=0; i < size; i+=read) {
                read = file.read(buffer, i, 1024);
                read = out2file.read(buffer2, i, 1024);
                bool FAIL = false;
                for(int j=0; j<read; j++) {
                    if(buffer[j] != buffer2[j]) FAIL = true;
                }
                if(FAIL) { fail("FAILURE"); break; }
            }

            if(i >= size) {
                success("SUCCESS");
            }
        }
    }

    clean();
    return 0;
}
