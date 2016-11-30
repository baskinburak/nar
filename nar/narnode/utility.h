#ifndef NAR_UTILITY_H
#define NAR_UTILITY_H
#include <iostream>
#include <string>
#include <nar/lib/Socket/Socket.h>
#include <crypto++/filters.h>

namespace nar {
    const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string get_string_sckt(int sockfd, int len);
    int get_int_sckt(int sockfd);
    void send_int_sckt(int sockfd, int val);
    void send_string_sckt(int sockfd, std::string str, int len);
	std::string get_message(Socket& skt);
	int readdata(Socket &sock, char *buf, int buflen);
	int readSckWriteFile(int filefd, Socket &skt, unsigned long fileSize );
    std::string byte_to_hex(byte* arr, int size);
    byte* hex_to_byte(std::string hex);
    void send_message(nar::Socket& skt, std::string message);
    void send_ipc_message(int sockfd, std::string message);
    std::string receive_ipc_message(int sockfd);
    std::string trim(std::string);
}

#endif
