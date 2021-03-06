#ifndef NAR_UTILITY_H
#define NAR_UTILITY_H
#include <iostream>
#include <string>
#include <nar/lib/Socket/Socket.h>
#include <crypto++/filters.h>
#include <string>
#include <nar/lib/Socket/USocket.h>
namespace nar {
    const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	std::string get_message(Socket& skt);
	std::string get_message(Socket* skt);
    std::string byte_to_hex(byte* arr, int size);
    byte* hex_to_byte(std::string hex);
    void send_message(nar::Socket& skt, std::string message);
    void send_message(nar::Socket* skt, std::string message);
    std::string trim(std::string);
    byte* string_to_byte(std::string &aes);
    void divide_nar_path(std::string &path, std::string& dir_path, std::string& file_name);
	bool isChunkExists(std::string& file_name);
}

#endif
