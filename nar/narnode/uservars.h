#ifndef NAR_USERVARS_H
#define NAR_USERVARS_H

#include <atomic>
#include <string>

namespace nar {
    class UserVariables {
        private:
            std::atomic<std::string> _current_directory;
            std::atomic<std::string> _username;
            std::atomic<std::string> _pass_aes;
        public:
            UserVariables(const std::string& current_directory, const std::string& username, const std::string& password);
            std::atomic<std::string>& get_current_directory();
            std::atomic<std::string>& get_username();
            std::atomic<std::string>& get_pass_aes();
    };
}

#endif
