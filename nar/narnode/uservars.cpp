#include "uservars.h"
nar::UserVariables::UserVariables(std::string current_directory, std::string username, std::string password): _current_directory(current_directory), _username(username) {
    while(_pass_aes.size() < 16) {
        _pass_aes += password;
    }
    _pass_aes.resize(16);
}

std::string& nar::UserVariables::get_current_directory() {
    return this->_current_directory;
}

std::string& nar::UserVariables::get_username() {
    return this->_username;
}

std::string& nar::UserVariables::get_pass_aes() {
    return this->_pass_aes;
}
