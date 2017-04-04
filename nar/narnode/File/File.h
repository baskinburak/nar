#ifndef NAR_FILE_H
#define NAR_FILE_H
#include <fstream>

namespace nar {
    /*
     * Utility class to write/read to/from files.
     *
     * @author: baskin
     * @privar: _file_handle, std::fstream, stores the fstream handle to the file.
     * @privar: _mode, std::string, stores the open mode of the file (r or w)
     * @privar: _file_path, std::string, path of the file
     * @privar: _isTemp, book, is this file temporary or not
     * @tested: no
    */
    class File {
        private:
            std::fstream _file_handle;
            std::string _mode;
            std::string _file_path;
            bool _is_temp;
        public:
            /*
             * Constructor. Opens the file given as file_path with mode.
             *
             * @author: baskin
             * @param: file_path, const char*, relative or absolute path to file. It is better practice to use full paths.
             * @param: mode, const char*, file open mode. Must be "r" for read, "w" for write.
             * @param: is_temp, bool, is this file temporary or not
             * @throws: nar::Exception::File::UnrecognizedFileMode, if file mode is different from "r" and "w".
             * @throws: nar::Exception::File::OpenFail, if file couldn't opened with given mode.
             * @tested: yes
             *
            */
            File(const char* file_path, const char* mode, bool is_temp);


            /*
             * Constructor. Opens the file given as file_path with mode.
             *
             * @author: baskin
             * @param: file_path, std::string, relative or absolute path to file. It is better practice to use full paths.
             * @param: mode, const char*, file open mode. Must be "r" for read, "w" for write.
             * @param: is_temp, bool, is this file temporary or not
             * @throws: nar::Exception::File::UnrecognizedFileMode, if file mode is different from "r" and "w".
             * @throws: nar::Exception::File::OpenFail, if file couldn't opened with given mode.
             * @tested: yes
             *
            */
            File(std::string file_path, const char* mode, bool is_temp);

            /*
             * Destructor. Closes the file stream.
             *
             * @author: baskin
             * @throws: nar::Exception::Unknown, if an unknown error is occured.
             * @tested: yes
             *
            */
            ~File();

            /*
             * Reads upto len bytes from the file starting from the given offset. Writes the result to buffer. Returns the number of bytes that are read.
             *
             * @author: baskin
             * @param: buffer, char*, A pointer that points to the memory location that read bytes will be written into.
             * @param: offset, unsigned long, Offset from the start of the file.
             * @param: len, unsigned long, The maximum amount of bytes that are requested to be read.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "w".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::File::OffsetOutOfBounds, if offset exceeds the file size.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::ReadError, if an unknown error is occured during read operation.
             * @return: unsigned long, Number of bytes that are read.
             * @tested: no, check for boundary conditions!!!
             *
            */
            unsigned long read(char* buffer, unsigned long offset, unsigned long len);

            /*
             * Writes len number of bytes from buffer, to file, starting from given offset of file.
             *
             * @author: baskin
             * @param: buffer, char*, A pointer that points to the memory location that contains the bytes that will be written into file.
             * @param: offset, unsigned long, The offset value in file starting from which the contents of buffer will be written into.
             * @param: len, unsigned long, Number of bytes that will be written.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "r".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::File::OffsetOutOfBounds, if offset exceeds the file size.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::WriteError, if an unknown error is occured during write operation.
             * @return: unsigned long, Number of bytes that are written.
             * @tested: yes
            */
            unsigned long write(char* buffer, unsigned long offset, unsigned long len);

            /*
             * Writes len number of bytes from buffer, to file, starting from the end of the file.
             *
             * @author: baskin
             * @param: buffer, char*, A pointer that points to the memory location that contains the bytes that will be written into file.
             * @param: len, unsigned long, Number of bytes that will be written.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "r".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::WriteError, if an unknown error is occured during write operation.
             * @return: unsigned long, Number of bytes that are written.
             * @tested: yes
            */
            unsigned long write(char* buffer, unsigned long len);

            /*
             * Returns the file size.
             *
             * @author: baskin
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::Unknown, if an unknown error is occured during seek and tell operations in fstream.
             * @return: unsigned long, File size in bytes.
             * @tested: yes
            */
            unsigned long size();

            /*
             * Closes the file stream.
             *
             * @author: baskin
             * @throws: nar::Exception::Unknown, if an unknown error is occured
             * @tested: yes
             *
            */
            void close();

            /*
             * Compress the given file and returns the compressed file
             *
             * @author: Fatih
             * @return: nar::File*, returns the temp file that contains compressed version
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "w".
             * @throws: nar::Exception::File::WrongMode, if tempfile is opened with mode "r".
             * @tested: yes
            */
            nar::File* compress();

            /*
             * Decompress the given file and returns the decompressed file
             *
             * @author: Fatih
             * @return: nar::File*, returns the temp file that contains decompressed version
             * @throws: nar::Exception::File::WrongMode, if compressed file is opened with mode "w".
             * @throws: nar::Exception::File::WrongMode, if the output file  is opened with mode "r".
             * @tested: yes
            */
            nar::File* decompress();

            nar::File* encrypt(std::string& aes);

            nar::File* decrypt(std::string& aes, std::string& fname);

            unsigned long long get_size();
    };
}

#endif
