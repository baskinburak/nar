#include <nar/narnode/utility.h>
#include <nar/narnode/Exception/exception.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>

std::string nar::get_message(nar::Socket& skt) {
    char buf[1025];
    int received = skt.recv(buf, 1024);
    if(received <= 0) throw nar::Exception("Connection Lost");
    int len = 0;
    int prev_len = 0;
    int idx = 0;
    for(; idx<received; idx++) {
        if(buf[idx] == ' ') {
            idx++;
            break;
        }
        if(buf[idx] > '9' || buf[idx] < '0') {
            std::cout << "bad request" << std::endl;
            skt.send((char*) "bad request", 11);
            skt.close();
            throw nar::Exception("such exception");
        }
        prev_len = len;
        len*=10;
        len += buf[idx] - '0';
        if(prev_len > len) {
            std::cout << "overflow" << std::endl;
            skt.send((char*) "integer overflow", 16);
            skt.close();
            throw nar::Exception("wowowo");
        }
    }
    std::string data;
    int cur;
    data.append(buf+idx, cur = std::min((received - idx + 1), len));
    len -= cur;

    while(len > 0) {
        int rec = skt.recv(buf, std::min(len, 1024));
        if(rec > 0) {
            data.append(buf, rec);
            len-=rec;
        } else {
            throw nar::Exception("wtf");
        }
    }
    return data;

}


std::string nar::get_string_sckt(int sockfd, int len) {
    std::string res;
    int idx = 0;
    int l;
    char buf[129];
    while(idx < len) {
        if((l = recv(sockfd, buf, 128, 0)) > 0) {
            res.append(buf, l);
            idx+=l;
        } else {
            throw nar::Exception("nar::get_string_sckt - Recv error.");
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
            if(buf[i] < '0' || buf[i] > '9')
                throw nar::Exception("nar::get_int_sckt - Not int");
            ret*=10;
            ret += buf[i] - '0';
        }
    } else {
        throw nar::Exception("nar::get_int_sckt - Recv error.");
    }
    return ret;
}

void nar::send_int_sckt(int sockfd, int val) {
    std::string val_str = std::to_string(val);

    if(send(sockfd, val_str.c_str(), val_str.size(), 0) == -1) {
        throw nar::Exception("nar::sent_int_sckt - Send error");
    }    
}

void nar::send_string_sckt(int sockfd, std::string str, int len) {
    int idx = 0;

    while(idx < len) {
        int ret = send(sockfd, str.c_str() + idx, len - idx, 0);
        if(ret == -1) {
            throw nar::Exception("narIPCClient::sendRequest - Send string error.");
        }
        idx += ret;
    }
}
