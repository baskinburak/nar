#ifndef NAR_USERVARS_H
#define NAR_USERVARS_H

#include <atomic>
#include <string>

namespace nar {
    class UserVariables {
        private:
            std::string _current_directory;
            std::string _username;
            std::string _pass_aes;
        public:
            UserVariables(std::string current_directory, std::string username, std::string password);
            std::string& get_current_directory();
            std::string& get_username();
            std::string& get_pass_aes();
    };
}

#endif
