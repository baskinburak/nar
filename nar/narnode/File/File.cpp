#include "File.h"
#include <algorithm>
#include <nar/lib/Exception/Exception.h>
nar::File::File(const char* file_path, const char* mode): _mode(mode) {
    std::string mod(mode);
    if(mod != "w" && mod != "r") {
        throw nar::Exception::File::UnrecognizedFileMode("File open mode unrecognized.", mode);
    }
    try {
        if(mod == "w")
            _file_handle.open(file_path, std::ios::out);
        else if(mod == "r")
            _file_handle.open(file_path, std::ios::in);
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::File::DoesNotExist(Exp.what(), file_path);
    }
}

nar::File::File(std::string file_path, const char* mode): _mode(mode) {
    std::string mod(mode);
    if(mod != "w" && mod != "r") {
        throw nar::Exception::File::UnrecognizedFileMode("File open mode unrecognized.", mode);
    }
    try {
        if(mod == "w")
            _file_handle.open(file_path.c_str(), std::ios::out);
        else if(mod == "r")
            _file_handle.open(file_path.c_str(), std::ios::in);
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::File::DoesNotExist(Exp.what(), file_path.c_str());
    }
}

nar::File::~File() {
    _file_handle.close();
}

int nar::File::read(char* buffer, int offset, int len) {
    if(_mode != "r") {
        throw nar::Exception::File::WrongMode("File is not opened with 'r'", _mode.c_str());
    }
    if(_file_handle.is_open()) {
        int file_len = size();
        if(offset >= file_len)
            throw nar::Exception::File::OffsetOutOfBounds("Offset is greater than file length in read()", offset);
        if(len < 0)
            throw nar::Exception::DomainError::Negative("read() size given negative", len);
        int read_len = std::min(file_len - offset, len);
        _file_handle.seekg(offset, _file_handle.beg);
        _file_handle.read(buffer, read_len);
        return read_len;
    } else {
        throw nar::Exception::File::NotOpen("read() called before file is opened.");
    }
}

int nar::File::write(char* buffer, int offset, int len) {
    if(_mode != "w") {
        throw nar::Exception::File::WrongMode("File is not opened with 'w'", _mode.c_str());
    }
    if(_file_handle.is_open()) {
        int file_len = size();
        if(offset >= file_len)
            throw nar::Exception::File::OffsetOutOfBounds("Offset is greater than file length in write()", offset);
        if(len < 0)
            throw nar::Exception::DomainError::Negative("write() size given negative", len);
        _file_handle.seekg(offset, _file_handle.beg);
        _file_handle.write(buffer, len);
        return len;
    } else {
        throw nar::Exception::File::NotOpen("write() called before file is opened.");
    }
}

int nar::File::write(char* buffer, int len) {
    if(_mode != "w") {
        throw nar::Exception::File::WrongMode("File is not opened with 'w'", _mode.c_str());
    }
    if(_file_handle.is_open()) {
        if(len < 0)
            throw nar::Exception::DomainError::Negative("write() size given negative", len);
        _file_handle.seekg(0, _file_handle.end);
        _file_handle.write(buffer, len);
        return len;
    } else {
        throw nar::Exception::File::NotOpen("write() called before file is opened.");
    }
}

unsigned long nar::File::size() {
    if(_file_handle.is_open()) {
        _file_handle.seekg(0, _file_handle.end);
        return _file_handle.tellg();
    } else {
        throw nar::Exception::File::NotOpen("size() called before file is opened.");
    }
}
