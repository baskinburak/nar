#ifndef NAR_CLITASKSS_H
#define NAR_CLITASKSS_H
#include <string>
#include <nar/narnode/global.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>


namespace nar {
    namespace CLITasks {
        void nar_ls(std::string dir_name, std::string username, std::string password, std::string curdir);
        void nar_pull(std::string file_name, std::string dir_name,std::string username, std::string password, std::string curdir);
        void nar_push(std::string file_name, std::string username, std::string password, std::string curdir);
        void nar_register(std::string username, std::string password);
        void nar_status(std::string username, std::string password);
        void nar_mkdir(std::string dir_name, std::string username, std::string password, std::string curdir);
        void nar_delete_file(std::string file_name, std::string username, std::string password, std::string curdir);
    }
}

#endif
