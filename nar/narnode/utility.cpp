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

std::string nar::get_message(nar::Socket& skt) {
    char buf[1035];
    int received = skt.recv(buf, 10);
    if(received <= 0) {
        std::cout << "connection lost"<< std::endl; exit(0);
    }
    int len = 0;
    int prev_len = 0;
    int idx = 0;
    for(; idx<received; idx++) {
        if(buf[idx] == ' ') {
            idx++;
            break;
        }
        if(buf[idx] > '9' || buf[idx] < '0') {
            skt.send((char*) "bad request", 11);
            skt.close();
        std::cout << "such exception"<< std::endl; exit(0);
        }
        prev_len = len;
        len*=10;
        len += buf[idx] - '0';
        if(prev_len > len) {
            skt.send((char*) "integer overflow", 16);
            skt.close();
        std::cout << "wowowo    "<< std::endl; exit(0);
        }
    }

    std::cout << "len: " << len << std::endl;
    std::string data;
    int cur;
    data.append(buf+idx, cur = std::min((received - idx), len));
    len -= cur;

    while(len > 0) {
        int rec = skt.recv(buf, std::min(len, std::min(len, 1024)));
        if(rec > 0) {
            data.append(buf, rec);
            len-=rec;
        } else {
        std::cout << "wtf"<< std::endl; exit(0);
        }
    }
    std::cout << "data: " << data << std::endl;
    return data;
}


std::string nar::get_string_sckt(int sockfd, int len) {
    std::string res;
    int idx = 0;
    int l = 0;
    char buf[129];

    int val = len;
    while(idx < len) {
        if((l = recv(sockfd, buf, std::min(val, 128), 0)) > 0) {
            res.append(buf, l);
			//std::cout<<"res "<<res<<std::endl;
            idx+=l;
			val -=l;
        } else {
            //throw nar::Exception("nar::get_string_sckt - Recv error.");
			std::cout << "nar::get_string_sckt - Recv error." << std::endl;
			break;
        }
    }
    return res;
}

int nar::get_int_sckt(int sockfd) {
    std::string res;
    int l;
    int ret = 0;
    char buf[129];
    if((l = recv(sockfd, buf, 128, 0)) > 0) {
        for(int i=0; i<l; i++) {
            if(buf[i] < '0' || buf[i] > '9') {
                std::cout << "not int"<< std::endl; exit(0);
            }
            ret*=10;
            ret += buf[i] - '0';
        }
    } else {
        std::cout << "recv error"<< std::endl; exit(0);
    }
    return ret;
}

void nar::send_int_sckt(int sockfd, int val) {
    std::string val_str = std::to_string(val);

    if(send(sockfd, val_str.c_str(), (int)val_str.size(), 0) == -1) {
        std::cout << "send error"<< std::endl; exit(0);
    }
}

int nar::readdata(nar::Socket &sock, char *buf, int buflen)
{
	//std::cout << "BufLen: " << buflen << std::endl;
    while (buflen > 0)
    {
	////std::cout << "Pbuf1: " << std::string(buf) << std::endl << std::endl;
        int num = sock.recv( buf, buflen);
	////std::cout << "Pbuf2: " << std::string(buf) << std::endl << std::endl;
	    if (num < 1)
        {
            return 0;
        }
        else if (num == 0)
            return 0;

        buf += num;
        buflen -= num;
    }


    return 1;
}

int nar::readdata(nar::USocket &sock, char *buf, int buflen)
{
	//std::cout << "BufLen: " << buflen << std::endl;
    while (buflen > 0)
    {
	////std::cout << "Pbuf1: " << std::string(buf) << std::endl << std::endl;
        int num = sock.recv(buf, buflen);
	////std::cout << "Pbuf2: " << std::string(buf) << std::endl << std::endl;

        buf += num;
        buflen -= num;
    }


    return 1;
}





void nar::send_string_sckt(int sockfd, std::string str, int len) {
    int idx = 0;

    while(idx < len) {
        int ret = send(sockfd, str.c_str() + idx, len - idx, 0);
        if(ret == -1) {
            std::cout << "send string error"<< std::endl; exit(0);
        }
        idx += ret;
    }
}

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


void nar::send_message(nar::Socket& skt, std::string message) {
    message = std::to_string((int)message.size()) + std::string(" ") + message;
    skt.send((char*)message.c_str(), (int)message.size());
}

void nar::send_message(nar::Socket* skt, std::string message) {
    message = std::to_string((int)message.size()) + std::string(" ") + message;
    skt->send((char*)message.c_str(), (int)message.size());
}

std::string nar::trim(std::string inp) {
    size_t first = inp.find_first_not_of(std::string(" \n"));
    if(std::string::npos == first)
        return inp;
    size_t last = inp.find_last_not_of(std::string(" \n"));
    return inp.substr(first, (last-first+1));
}
