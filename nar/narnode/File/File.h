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
     * @tested: no
    */
    class File {
        private:
            std::fstream _file_handle;
            std::string _mode;
        public:
            /*
             * Constructor. Opens the file given as file_path with mode.
             *
             * @author: baskin
             * @param: file_path, const char*, relative or absolute path to file. It is better practice to use full paths.
             * @param: mode, const char*, file open mode. Must be "r" for read, "w" for write.
             * @throws: nar::Exception::File::UnrecognizedFileMode, if file mode is different from "r" and "w".
             * @throws: nar::Exception::File::OpenFail, if file couldn't opened with given mode.
             * @tested: yes
             *
            */
            File(const char* file_path, const char* mode);


            /*
             * Constructor. Opens the file given as file_path with mode.
             *
             * @author: baskin
             * @param: file_path, std::string, relative or absolute path to file. It is better practice to use full paths.
             * @param: mode, const char*, file open mode. Must be "r" for read, "w" for write.
             * @throws: nar::Exception::File::UnrecognizedFileMode, if file mode is different from "r" and "w".
             * @throws: nar::Exception::File::OpenFail, if file couldn't opened with given mode.
             * @tested: yes
             *
            */
            File(std::string file_path, const char* mode);

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
             * @param: offset, int, Offset from the start of the file.
             * @param: len, int, The maximum amount of bytes that are requested to be read.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "w".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::File::OffsetOutOfBounds, if offset exceeds the file size.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::ReadError, if an unknown error is occured during read operation.
             * @return: int, Number of bytes that are read.
             * @tested: no, check for boundary conditions!!!
             *
            */
            int read(char* buffer, int offset, int len);

            /*
             * Writes len number of bytes from buffer, to file, starting from given offset of file.
             *
             * @author: baskin
             * @param: buffer, char*, A pointer that points to the memory location that contains the bytes that will be written into file.
             * @param: offset, int, The offset value in file starting from which the contents of buffer will be written into.
             * @param: len, int, Number of bytes that will be written.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "r".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::File::OffsetOutOfBounds, if offset exceeds the file size.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::WriteError, if an unknown error is occured during write operation.
             * @return: int, Number of bytes that are written.
             * @tested: yes
            */
            int write(char* buffer, int offset, int len);

            /*
             * Writes len number of bytes from buffer, to file, starting from the end of the file.
             *
             * @author: baskin
             * @param: buffer, char*, A pointer that points to the memory location that contains the bytes that will be written into file.
             * @param: len, int, Number of bytes that will be written.
             * @throws: nar::Exception::File::WrongMode, if file is opened with mode "r".
             * @throws: nar::Exception::File::NotOpen, if file is not opened prior to the call.
             * @throws: nar::Exception::DomainError::Negative, if len is less than 0.
             * @throws: nar::Exception::File::WriteError, if an unknown error is occured during write operation.
             * @return: int, Number of bytes that are written.
             * @tested: yes
            */
            int write(char* buffer, int len);

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
    };
}

#endif
