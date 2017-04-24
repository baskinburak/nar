#include "clitasks.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <string>

std::pair<std::string, std::string> get_uname_pw() {
    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    return std::make_pair(username, password);
}

int main(int argc, char* argv[]){
    if(argc < 2) {
        return 0;
    }
    std::string first_arg(argv[1]);

    if(first_arg == std::string("push")) {
        if(argc < 3) {
            return 0;
        }
        std::string file_name(argv[2]);
        auto unamepwd = get_uname_pw();
        nar::CLITasks::nar_push(file_name, unamepwd.first, unamepwd.second, std::string("/"));

    } else if(first_arg == std::string("ls")) {
        auto unamepwd = get_uname_pw();
        if(argc < 3) {
            nar::CLITasks::nar_ls(std::string(""), unamepwd.first, unamepwd.second, std::string("/"));
        }
        else if(argc < 4) {
            std::string dir_name(argv[2]);
            nar::CLITasks::nar_ls(dir_name, unamepwd.first, unamepwd.second, std::string("/"));
        }

    } else if(first_arg == std::string("cd")) {
        auto unamepwd = get_uname_pw();
        if(argc < 3) {
            return 0;
        }
        std::string dir_name(argv[2]);
        //nar::CLITasks::nar_cd( dir_name,  unamepwd.first, unamepwd.second, std::string("/"));

    } else if(first_arg == std::string("pull")) {
        if(argc < 4) {
            return 0;
        }
        auto unamepwd = get_uname_pw();
        std::string file_name(argv[2]);
        std::string dir_name(argv[3]);
        nar::CLITasks::nar_pull(file_name,dir_name, unamepwd.first, unamepwd.second, std::string("/"));

    } else if(first_arg == std::string("register")) {
        auto unamepwd = get_uname_pw();
        nar::CLITasks::nar_register(unamepwd.first, unamepwd.second);

    } else if(first_arg == std::string("status")) {
        nar::CLITasks::nar_status();
    } else if(first_arg == std::string("mkdir")) {
        if(argc < 3 ){
            return 0;
        }
        std::string dir_name(argv[2]);
        auto unamepwd = get_uname_pw();
        nar::CLITasks::nar_mkdir(dir_name, unamepwd.first, unamepwd.second, std::string("/"));
    }
    return 0;
}
