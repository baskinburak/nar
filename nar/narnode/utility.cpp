#include <nar/narnode/utility.h>
#include <nar/narnode/Exception/exception.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <algorithm>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <string>
#include <sstream>

std::string nar::get_message(nar::Socket& skt) {
    char buf[1035];
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
            //std::cout << "bad request" << std::endl;
            skt.send((char*) "bad request", 11);
            skt.close();
            throw nar::Exception("such exception");
        }
        prev_len = len;
        len*=10;
        len += buf[idx] - '0';
        if(prev_len > len) {
            //std::cout << "overflow" << std::endl;
            skt.send((char*) "integer overflow", 16);
            skt.close();
            throw nar::Exception("wowowo");
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
            throw nar::Exception("wtf");
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

    if(send(sockfd, val_str.c_str(), (int)val_str.size(), 0) == -1) {
        throw nar::Exception("nar::sent_int_sckt - Send error");
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

int nar::senddata(nar::Socket &sock, char *buf, int buflen)
{
     std::cout << "siktirtmeyin" << std::endl; // bunu sil
    exit(0);
}

int nar::senddata(nar::USocket &sock, char *buf, int buflen)
{
     char *pbuf = ( char *) buf;

    while (buflen > 0)
    {
		//std::cout <<" pBuf: " << "bufLen: "<<buflen << "xxx" <<std::string(pbuf) << std::endl<<std::endl;
        int num = sock.send( pbuf, buflen);
        if (num < 0)
        {
			//std::cout << "IAM HEREEEEé!!!!!!!!!!!!" << std::endl;
            return 0;
        }

        pbuf += num;
        buflen -= num;
    }

    return 1;
}


int nar::readFileWriteSck( nar::FileCryptor &file, nar::Socket &skt, unsigned long fileSize, unsigned long _offset) {
    std::cout << "guzel migrate edin" << std::endl; // bunu sil
	exit(0);
}

int nar::readFileWriteSck( nar::FileCryptor &file, nar::USocket &skt, unsigned long fileSize, unsigned long _offset) {
	char *buffer = new char[1000200];
	//std::cout << "fileSizeinit " << fileSize << std::endl;
	int offset = _offset;
	do
	{
		int num = std::min(fileSize, (unsigned long int )1000000);
		size_t readd = file.getBytes(offset, num, buffer);
		offset += readd;


		nar::senddata(skt,buffer,readd);
		

		fileSize -= readd;
	} while (fileSize > 0);
  delete[] buffer;
	return 1;
}



int nar::readSckWriteFile(int filefd, nar::Socket &skt, unsigned long fileSize ){

	char *buffer = new char[1024];

	std::cout << "FILE SIZE HERE !é!!! : " << fileSize << std::endl;

    unsigned long total = 0;
    do
    {
        int num = std::min(fileSize, (unsigned long int)1024);
		//std::cout << "Num: "<< num << std::endl;
        if (! nar::readdata(skt,buffer, num) ) {
		   std::cout << "Read Data Failed from peer Skt" << std::endl;
		   return 0;
		}
        total+=num;

		////std::cout <<  "buffer: "<<std::string(buffer) << std::endl << std::endl << std::endl ;


        nar::FileKeeper::writeToFile( filefd,  num, buffer);



        fileSize -= num;

	}
    while (fileSize > 0);
    std::cout<<"total ->>>> "<<total<<std::endl;
	delete buffer;

	return 1;
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

void nar::send_ipc_message(int sockfd, std::string message) {
    int len = message.size();
    send_int_sckt(sockfd, len);
    std::string ok = get_string_sckt(sockfd, 2);
    send_string_sckt(sockfd, message, len);
    ok = get_string_sckt(sockfd, 2);
}

std::string nar::trim(std::string inp) {
    size_t first = inp.find_first_not_of(' ');
    if(std::string::npos == first)
        return inp;
    size_t last = inp.find_last_not_of(' ');
    return inp.substr(first, (last-first+1));
}

std::string nar::receive_ipc_message(int sockfd) {
    int len = get_int_sckt(sockfd);
    send_string_sckt(sockfd, std::string("OK"), 2);
    std::string message = get_string_sckt(sockfd, len);
    send_string_sckt(sockfd, std::string("OK"), 2);
    return message;
}
