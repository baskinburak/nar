#include "File.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include "base64.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/filesystem.hpp>
#include "../../lib/Exception/Exception.h"
#include <stdio.h>

nar::File::File(const char* file_path, const char* mode, bool is_temp): _mode(mode), _is_temp(is_temp), _file_path(file_path){

    std::string mod(mode);
    if(mod != "w" && mod != "r") {
        throw nar::Exception::File::UnrecognizedFileMode("File open mode unrecognized.", mode);
    }
    try {
        if(mod == "w")
            _file_handle.open(file_path, std::ios::out | std::ios::binary);
        else if(mod == "r")
            _file_handle.open(file_path, std::ios::in | std::ios::binary);
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::File::OpenFail(Exp.what(), file_path);
    }
}

nar::File::File(std::string file_path, const char* mode, bool is_temp): _mode(mode), _is_temp(is_temp), _file_path(file_path) {
    std::string mod(mode);
    if(mod != "w" && mod != "r") {
        throw nar::Exception::File::UnrecognizedFileMode("File open mode unrecognized.", mode);
    }
    try {
        if(mod == "w")
            _file_handle.open(file_path.c_str(), std::ios::out | std::ios::binary);
        else if(mod == "r")
            _file_handle.open(file_path.c_str(), std::ios::in | std::ios::binary);
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::File::OpenFail(Exp.what(), file_path.c_str());
    }
}

nar::File::~File() {
    try {
        _file_handle.close();
        if(_is_temp) {
            boost::filesystem::remove(_file_path.c_str());
        }
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
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
        try {
            _file_handle.seekg(offset, _file_handle.beg);
            _file_handle.read(buffer, read_len);
        } catch(std::ios_base::failure& Exp) {
            throw nar::Exception::File::ReadError(Exp.what(), offset, read_len);
        }
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
        try {
            _file_handle.seekg(offset, _file_handle.beg);
            _file_handle.write(buffer, len);
        } catch(std::ios_base::failure& Exp) {
            throw nar::Exception::File::WriteError(Exp.what(), offset, len);
        }
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
        try {
            _file_handle.seekg(0, _file_handle.end);
            _file_handle.write(buffer, len);
        } catch(std::ios_base::failure& Exp) {
            throw nar::Exception::File::WriteError(Exp.what(), -1, len);
        }
        return len;
    } else {
        throw nar::Exception::File::NotOpen("write() called before file is opened.");
    }
}

unsigned long nar::File::size() {
    if(_file_handle.is_open()) {
        try {
            _file_handle.seekg(0, _file_handle.end);
            return _file_handle.tellg();
        } catch(std::ios_base::failure& Exp) {
            throw nar::Exception::Unknown(Exp.what());
        }
    } else {
        throw nar::Exception::File::NotOpen("size() called before file is opened.");
    }
}

void nar::File::close() {
    try {
        _file_handle.close();
        if(_is_temp) {
            boost::filesystem::remove(_file_path.c_str());
        }
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
}

nar::File* nar::File::compress() {
    boost::filesystem::path temp;
    try {
        temp = boost::filesystem::unique_path();
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
    std::string temp_file =temp.native();
    temp_file += std::string(".z");
    nar::File tempfile(temp_file, "w", false);
    if(this->_mode != "r") {
        throw nar::Exception::File::WrongMode("File is not opened with 'r'", _mode.c_str());
    }
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_compressor());
    in.push(this->_file_handle);
    boost::iostreams::copy(in, tempfile._file_handle);
    nar::File* r_version = new nar::File(temp_file, "r", true);
    return  r_version;
}
nar::File* nar::File::decompress() {
    boost::filesystem::path temp;
    try {
        temp = boost::filesystem::unique_path();
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
    std::string temp_file =temp.native();
    temp_file += std::string(".txt");
    nar::File tempfile(temp_file, "w", false);
    if(this->_mode != "r") {
        throw nar::Exception::File::WrongMode("Compressed file should have opened with 'r'", _mode.c_str());
    }
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(this->_file_handle);
    boost::iostreams::copy(in, tempfile._file_handle);

    nar::File* r_version = new nar::File(temp_file, "r", true);
    return  r_version;
}
