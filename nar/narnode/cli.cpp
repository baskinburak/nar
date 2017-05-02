#include "clitasks.h"

#include <boost/filesystem.hpp>
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
        boost::filesystem::path file(file_name);
        try {
            file = boost::filesystem::canonical(file);
        } catch (boost::filesystem::filesystem_error er) {
            std::cout << "Path '" + er.path1().string() + "' does not exist or does not name a file"  << '\n';
            return 0;
        }
        if( !boost::filesystem::exists(file) ) {
            std::cout << "Path does not exist" << '\n';
            return 0;
        }
        if( boost::filesystem::is_directory(file) ) {
            std::cout << "Path does not name a file" << std::endl;
            return 0;
        }

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
    } else if(first_arg == std::string("delete_file")) {
        auto  unamepwd = get_uname_pw();
        if(argc < 3) {
            return 0;
        }
        std::string file_name(argv[2]);
        nar::CLITasks::nar_delete_file( file_name,  unamepwd.first, unamepwd.second, std::string("/"));

    } else if(first_arg == std::string("pull")) {
        if(argc < 4) {
            return 0;
        }
        auto unamepwd = get_uname_pw();
        std::string file_name(argv[2]);
        std::string dir_name(argv[3]);
        boost::filesystem::path dir(dir_name);
        try {
            dir = boost::filesystem::canonical(dir);
        } catch (boost::filesystem::filesystem_error er) {
            std::cout << "Path '" + er.path1().string() + "' does not exist or does not name a folder"  << '\n';
            return 0;
        }
        if( !boost::filesystem::exists(dir) ) {
            std::cout << "Path does not exist" << '\n';
            return 0;
        }
        if( !boost::filesystem::is_directory(dir) ) {
            std::cout << "Path is not a directory" << std::endl;
            return 0;
        }
        
        nar::CLITasks::nar_pull(file_name, dir.string(), unamepwd.first, unamepwd.second, std::string("/"));

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
