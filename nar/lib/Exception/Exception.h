#ifndef NAR_EXCEPTION_H
#define NAR_EXCEPTION_H
#include <stdexcept>
#include <string>

namespace nar {
    class Exception {
        private:
            std::string _message;
        public:
            Exception(const char* mess) noexcept: _message(mess) {}
            std::string what() const { return _message; }
    };

    namespace DomainError {
        class Negative : public nar::Exception {
            private:
                int _value;
            public:
                Negative(const char* mess, int value): nar::Exception(mess), _value(value) {}
                int get_value() const { return _value; }
        };
    };

    namespace File {
        class DoesNotExist : public nar::Exception {
            private:
                std::string _file_path;
            public:
                DoesNotExist(const char* mess, const char* file_path): nar::Exception(mess), _file_path(file_path) {}
                std::string get_file_path() const { return _file_path; }
        };

        class OffsetOutOfBounds : public nar::Exception {
            private:
                int _offset;
            public:
                OffsetOutOfBounds(const char* mess, int offset): nar::Exception(mess), _offset(offset) {}
                int get_offset() const { return _offset; }
        };

        class NotOpen : public nar::Exception {
            public:
                NotOpen(const char* mess): nar::Exception(mess) {}
        };

        class NothingToRead : public nar::Exception {
            private:
                int _file_len;
                int _offset;
                int _len;
            public:
                NothingToRead(const char* mess, int file_len, int offset, int len): nar::Exception(mess), _file_len(file_len), _offset(offset), _len(len) {}
                int get_file_len() const { return _file_len; }
                int get_offset() const { return _offset; }
                int get_len() const { return _len; }
        };
    };
}

#endif
