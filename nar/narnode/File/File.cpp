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
#include <crypto++/gcm.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>
#include <stdio.h>
#include <nar/narnode/utility.h>
//#include "../utility.h"

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

unsigned long nar::File::read(char* buffer, unsigned long offset, unsigned long len) {
    if(_mode != "r") {
        throw nar::Exception::File::WrongMode("File is not opened with 'r'", _mode.c_str());
    }
    if(_file_handle.is_open()) {
        unsigned long file_len = size();
        if(offset >= file_len)
            throw nar::Exception::File::OffsetOutOfBounds("Offset is greater than file length in read()", offset);
        if(len < 0)
            throw nar::Exception::DomainError::Negative("read() size given negative", len);
        unsigned long read_len = std::min(file_len - offset, len);
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

unsigned long nar::File::write(char* buffer, unsigned long offset, unsigned long len) {
    if(_mode != "w") {
        throw nar::Exception::File::WrongMode("File is not opened with 'w'", _mode.c_str());
    }
    if(_file_handle.is_open()) {
        unsigned long file_len = size();
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

unsigned long nar::File::write(char* buffer, unsigned long len) {
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

nar::File* nar::File::encrypt(std::string& aes) {

    boost::filesystem::path temp;
    try {
        temp = boost::filesystem::unique_path();
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
    if(this->_mode != "r") {
        throw nar::Exception::File::WrongMode("File is not opened with 'r'", _mode.c_str());
    }

    std::string temp_native = temp.native();
    temp_native.append(".enc");
    std::cout<<temp_native<<std::endl;
    nar::File out(temp_native,"w",false);

    byte iv[256];
    CryptoPP::AutoSeededRandomPool pool;
    pool.GenerateBlock(iv, 256);
    std::string ivStr = byte_to_hex(iv,256);
    out._file_handle << ivStr;


    const int TAG_SIZE = 12;

    byte* _aes = string_to_byte(aes);
    CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(_aes, 16, iv, 256);
    CryptoPP::FileSource ss1(this->_file_handle, true, new CryptoPP::AuthenticatedEncryptionFilter(enc, new CryptoPP::FileSink(out._file_handle/*binary*/),false,TAG_SIZE));
    out.close();
    nar::File* out2 = new nar::File(temp_native,"r",true);
    return out2;
}

nar::File* nar::File::decrypt(std::string& aes, std::string& fname) {
    using namespace CryptoPP;
    if(this->_mode != "r") {
        throw nar::Exception::File::WrongMode("File is not opened with 'r'", _mode.c_str());
    }
    nar::File out(fname.c_str(),"w",false);

    std::string hexIv;
    for(int ind=0; ind<512;++ind) {
        hexIv.append(1, this->_file_handle.get());
    }
    byte* iv = hex_to_byte(hexIv);
    CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
    byte* _aes = string_to_byte(aes);
    dec.SetKeyWithIV(_aes, 16, iv, 256);

    const int TAG_SIZE = 12;
    CryptoPP::FileSource ss(this->_file_handle, true, new CryptoPP::AuthenticatedDecryptionFilter(dec, new CryptoPP::FileSink(out._file_handle),CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, TAG_SIZE));
    nar::File* out2 = new nar::File(fname,"r",false);
    return out2;

}
unsigned long long nar::File::get_size() {
    boost::filesystem::path temp(this->_file_path);
    return boost::filesystem::file_size(temp);
}
