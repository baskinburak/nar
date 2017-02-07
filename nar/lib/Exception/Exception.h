#ifndef NAR_EXCEPTION_H
#define NAR_EXCEPTION_H
#include <stdexcept>
#include <string>

namespace nar {
    namespace Exception {
        class ExcpBase {
            private:
                std::string _message;
            public:
                ExcpBase(const char* mess): _message(mess) {}
                std::string what() const { return _message; }
        };

        namespace DomainError {
            class Negative : public nar::Exception::ExcpBase {
                private:
                    int _value;
                public:
                    Negative(const char* mess, int value): nar::Exception::ExcpBase(mess), _value(value) {}
                    int get_value() const { return _value; }
            };
        };

        namespace File {
            class WrongMode : public nar::Exception::ExcpBase {
                private:
                    std::string _mode;
                public:
                    WrongMode(const char* mess, const char* mode): nar::Exception::ExcpBase(mess), _mode(mode) {}
                    std::string get_mode() const { return _mode; }
            };
            class UnrecognizedFileMode : public nar::Exception::ExcpBase {
                private:
                    std::string _mode;
                public:
                    UnrecognizedFileMode(const char* mess, const char* mode): nar::Exception::ExcpBase(mess), _mode(mode) {}
                    std::string get_mode() const { return _mode; }
            };
            class DoesNotExist : public nar::Exception::ExcpBase {
                private:
                    std::string _file_path;
                public:
                    DoesNotExist(const char* mess, const char* file_path): nar::Exception::ExcpBase(mess), _file_path(file_path) {}
                    std::string get_file_path() const { return _file_path; }
            };

            class OffsetOutOfBounds : public nar::Exception::ExcpBase {
                private:
                    int _offset;
                public:
                    OffsetOutOfBounds(const char* mess, int offset): nar::Exception::ExcpBase(mess), _offset(offset) {}
                    int get_offset() const { return _offset; }
            };

            class NotOpen : public nar::Exception::ExcpBase {
                public:
                    NotOpen(const char* mess): nar::Exception::ExcpBase(mess) {}
            };

            class NothingToRead : public nar::Exception::ExcpBase {
                private:
                    int _file_len;
                    int _offset;
                    int _len;
                public:
                    NothingToRead(const char* mess, int file_len, int offset, int len): nar::Exception::ExcpBase(mess), _file_len(file_len), _offset(offset), _len(len) {}
                    int get_file_len() const { return _file_len; }
                    int get_offset() const { return _offset; }
                    int get_len() const { return _len; }
            };
        };
    };
};

#endif
