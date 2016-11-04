#include <iostream>

int nar_error(int err_id) {
    if(err_id == 1) {
        std::cout << "[narCommandLineArgumentError] Command needed." << std::endl;
    } else if(err_id == 2) {
        std::cout << "[narPushArgumentError] Please provide a file to push" << std::endl;
    }
    return -1;
}


void nar_push_file(std::string file_name) {
    
}

int main(int argc, char* argv[]){
    if(argc < 2) {
        return nar_error(1);
    }

    std::string first_arg(argv[1]);

    if(first_arg == std::string("push")) {
        if(argc < 3) {
            return nar_error(2);
        }

        std::string file_name(argv[2]);
        nar_push_file(file_name);
    }

    return 0;
}
