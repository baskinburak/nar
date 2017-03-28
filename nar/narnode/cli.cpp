#include <nar/narnode/CLITasks/nar_config.h>
#include <nar/narnode/CLITasks/nar_ls.h>
#include <nar/narnode/CLITasks/nar_pull_file.h>
#include <nar/narnode/CLITasks/nar_push_file.h>
#include <nar/narnode/CLITasks/nar_register.h>
#include <nar/narnode/CLITasks/nar_status.h>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <string>

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
        nar::CLITasks::nar_push_file(file_name);

    } else if(first_arg == std::string("ls")) {
        if(argc < 3) {
            nar::CLITasks::nar_ls(std::string(""));
        }
        else if(argc < 4){
            std::string dir_name(argv[2]);
            nar::CLITasks::nar_ls(dir_name);
        }

    } else if(first_arg == std::string("pull")) {
        if(argc < 3) {
            return 0;
        }
        std::string file_name(argv[2]);
        nar::CLITasks::nar_pull_file(file_name);

    } else if(first_arg == std::string("register")) {
        std::string uname;
        std::cout << "Enter a username: ";
        std::cin >> uname;
        nar::CLITasks::nar_register(uname);

    } else if(first_arg == std::string("config")) {
        if(argc < 3) {
            return 0;
        }
        for(int i=2; i<argc; i++) {
            std::string conf(argv[i]);
            int eq = conf.find('=');
            std::string left = conf.substr(0, eq);
            std::string right = conf.substr(eq+1);
            nar::CLITasks::nar_config(left, right);
        }

    } else if(first_arg == std::string("status")) {
        nar::CLITasks::nar_status();
    }
    return 0;
}
