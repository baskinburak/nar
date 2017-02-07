#ifndef NAR_FILE_H
#define NAR_FILE_H
#include <fstream>
namespace nar {
    /*
     * Utility class to write/read to/from files.
     *
     * @author: baskin
     * @privar: _file_handle, std::fstream, stores the fstream handle to the file.
     * @tested: no
     * @todo: everything
    */
    class File {
        private:
            std::fstream _file_handle;
            std::string _mode;
        public:
            /*
             * Default constructor. This should do nothing.
             *
             * @author: baskin
             * @tested: no
            */
            File(const char* file_path, const char* mode);
            File(std::string file_path, const char* mode);
            ~File();
            int read(char* buffer, int offset, int len);
            int write(char* buffer, int offset, int len);
            int write(char* buffer, int len);
            unsigned long size();
    };
}

#endif
