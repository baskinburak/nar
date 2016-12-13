#ifndef NAR_GLOBAL_H
#define NAR_GLOBAL_H
#include <mutex>
#include <string>
#include <iostream>

namespace nar {
    class Global {
        private:
            std::mutex read_mtx;
            std::mutex write_mtx;
            int read_count = 0;
            std::string username;
            std::string cur_dir;
			std::string narServerIP;
            std::string narFolder;
			int narServerPort;
            void read_start();
            void read_end();
            void write_start();
            void write_end();
        public:
            Global() {
                set_curdir("/");
                std::cout << "Server IP: " << std::endl;
                std::string ip;
                std::cin >> ip;
                set_narServerIp(ip);
                std::cout << "Thanks." << std::endl;
                set_narServerPort(12345);
                set_curdir("/");
            }
            std::string get_username();
            void set_username(std::string uname);
            std::string get_curdir();
            void set_curdir(std::string cd);
			std::string get_narServerIp();
			int get_narServerPort();
			void set_narServerIp(std::string ip);
			void set_narServerPort(int port);
            void set_narFolder(std::string fold);
            std::string get_narFolder();
    };
}

#endif
