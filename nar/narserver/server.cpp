#include <map>
#include <nar/lib/Socket/Socket.h>
#include <thread>
#include <iostream>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/narnode/utility.h>
#include <nar/narnode/Exception/exception.h>

std::map<std::string, nar::Socket> peer_keepalives;

std::string get_message(nar::Socket& skt) {
    char buf[1025];
    int received = skt.recv(buf, 1024);
    if(received <= 0) throw nar::Exception("damn son");
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

void handle_connection(nar::Socket skt, int id) {
    std::string request = get_message(skt);

    std::cout << request << std::endl;

    skt.close();
}

int main(int argc, char *argv[])
{
    nar::Socket entry_skt;
    entry_skt.create();
    entry_skt.bind(12345);
    entry_skt.listen();
    int i = 0;
    while(true) {
        nar::Socket new_skt;
        entry_skt.accept(new_skt, NULL);

        std::thread thr(&handle_connection, new_skt, i++);
        thr.detach();
    }    	
	return 0;
}
