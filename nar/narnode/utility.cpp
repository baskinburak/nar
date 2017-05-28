#include <nar/narnode/utility.h>
#include <nar/lib/Exception/Exception.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>

std::string nar::get_message(nar::Socket* skt) {
    return get_message(*skt);
}

void nar::divide_nar_path (std::string& dir_name, std::string &dir_path, std::string& file_name) {
    std::vector<std::string> names;
    std::size_t found;
    std::size_t last_found;
    std::string temp;
    std::string sql_string = "";
    if((dir_name[dir_name.length()-1] == '/') && dir_name.length()>1 ){
        dir_name = dir_name.substr(0,dir_name.length()-1);
    }
    if((dir_name[0] == '/') && dir_name.length()>1){
        found = dir_name.find("/",1);
        last_found = 1;
    }
    else{
        found = dir_name.find("/");
        last_found = 0;
    }

    while(found != std::string::npos){
        if(found == 0 && last_found == 0){
            temp = dir_name.substr(last_found,1);
            last_found = found+1;
            found = dir_name.find("/",last_found);
            names.push_back(temp);
        }
        else{
            temp = dir_name.substr(last_found,found-last_found);
            last_found = found+1;
            found = dir_name.find("/",last_found);
            names.push_back(temp);
        }

    }
    if(dir_name.length() != 1){
        temp = dir_name.substr(last_found);
        names.push_back(temp);
    }
    dir_path =  std::string("");
    file_name = std::string("");
    for(int i=0;i<names.size();i++) {
        if(i == names.size()-1) {
            file_name = names[i];
        }
        else {
            dir_path += names[i]+ std::string("/");

        }
    }
}


/* LOW LEVEL SOCKET MESSAGE EXCHANGE
*
* CODES
* \x01 -> [ERROR]Denotes that the start of message is not integer.
* \x02 -> [ERROR]Integer overflow, message too big.
* \x03 -> [SUCCESS]
*
*/



/*
 *
 *
 * @throws: nar::Exception::Socket::WrongSocketType, if socket is a server socket (acceptor)
 * @throws: nar::Exception::Socket::SystemError, if socket is closed or an unknown error is encountered
 * @throws: nar::Exception::LowLevelMessaging::NoSize("Message does not contain a size field."), if received message does not have a size field.
 * @throws: nar::Exception::LowLevelMessaging::SizeIntOverflow("Size is too big."), if received message size overflows int.
 *
 *
 *
*/
std::string nar::get_message(nar::Socket& skt) {
    static const char* codes = "\x01\x02\x03";
    char buf[1035];
    char nmin = 0;
    int received = skt.recv(buf, 10);

    int len = 0;
    int prev_len = 0;
    int idx = 0;
    skt.recv(&nmin, 1);
    while(nmin != ' ') {
        if(nmin > '9' || nmin < '0') {
            skt.send(codes, 1);
            throw nar::Exception::LowLevelMessaging::NoSize("Message does not contain a size field.");
        }
        prev_len = len;
        len*=10;
        len += nmin - '0';
        if(prev_len > len) {
            skt.send(codes+1, 1);
            throw nar::Exception::LowLevelMessaging::SizeIntOverflow("Size is too big.");
        }
        skt.recv(&nmin, 1);
    }

    std::string data;

    while(len > 0) {
        int rec = skt.recv(buf, std::min(len, std::min(len, 1024)));
        data.append(buf, rec);
        len -= rec;
    }

    skt.send(codes+2, 1);
    return data;
}


/*
 *
 *
 * @throws: nar::Exception::Socket::WrongSocketType, if socket is a server socket (acceptor)
 * @throws: nar::Exception::Socket::SystemError, if socket is closed or an unknown error is encountered
 * @throws: nar::Exception::LowLevelMessaging::SizeIntOverflow("Size is too big."), if message to be sent has size that overflows int of computer.
 * @throws: nar::Exception::LowLevelMessaging::FormatError("Format error."), if server reports that format is not accurate (NOT NORMAL !!!)
 * @throws: nar::Exception::LowLevelMessaging::ServerSizeIntOverflow("Message size is greater than max in of server", sizei), if message to be sent has size that overflows int of the server.
 *
*/
void nar::send_message(nar::Socket& skt, std::string message) {
    long long int sizell = message.size();
    int sizei = message.size();
    if(sizell != sizei) {
        throw nar::Exception::LowLevelMessaging::SizeIntOverflow("Message size is greater than max int of this computer");
    }
    message = std::to_string(message.size()) + std::string(" ") + message;

    skt.send((char*)message.c_str(), (int)message.size());
    char recv_code;
    skt.recv(&recv_code, 1);
    if(recv_code == 0x1) {
        throw nar::Exception::LowLevelMessaging::FormatError("Format error.");
    } else if(recv_code == 0x2) {
        throw nar::Exception::LowLevelMessaging::ServerSizeIntOverflow("Message size is greater than max in of server", sizei);
    } else if(recv_code == 0x3) {
        return;
    }
}

void nar::send_message(nar::Socket* skt, std::string message) {
    nar::send_message(*skt, message);
}

/* LOW LEVEL SOCKET MESSAGE EXCHANGE END */



byte* nar::string_to_byte(std::string &key) {
    int len = key.length();
    byte *aes = new byte[len];
    for(int i=0; i<len; i++) {
        aes[i] = (unsigned char )key[i];
    }
    return aes;
}

std::string nar::byte_to_hex(byte* arr, int len) {
    std::string res;
    for(int i=0; i<len; i++) {
        res.push_back(hexmap[(arr[i] & 0xf0) >> 4]);
        res.push_back(hexmap[arr[i] & 0xf]);
    }
    return res;
}

byte* nar::hex_to_byte(std::string hex) {
    byte* res = new byte[hex.size()/2];
    for(int i=0; i<hex.size()/2; i++) {
        if(hex[i<<1] >= '0' && hex[i<<1] <= '9')
            res[i] = ((hex[i<<1] - '0') << 4);
        else if(hex[i<<1] >= 'a' && hex[i<<1] <= 'f')
            res[i] = ((hex[i<<1] - 'a' + 10) << 4);

        if(hex[(i<<1) + 1] >= '0' &&  hex[(i<<1) + 1] <= '9')
            res[i] += hex[(i<<1) + 1] - '0';
        else if(hex[(i<<1) + 1] >= 'a' &&  hex[(i<<1) + 1] <= 'f')
            res[i] += hex[(i<<1) + 1] - 'a' + 10;
    }
    return res;
}


std::string nar::trim(std::string inp) {
    size_t first = inp.find_first_not_of(std::string(" \n"));
    if(std::string::npos == first)
        return inp;
    size_t last = inp.find_last_not_of(std::string(" \n"));
    return inp.substr(first, (last-first+1));
}

bool nar::isChunkExists(std::string& file_name) {
	bool isexists = true;
	boost::filesystem::path dir(file_name);
    try {
        dir = boost::filesystem::canonical(dir);
    } catch (boost::filesystem::filesystem_error er) {
        std::cout << er.what() << '\n';
		isexists = false;
        return isexists;
    }
	return isexists;
}

