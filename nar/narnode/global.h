#ifndef NAR_GLOBAL_H
#define NAR_GLOBAL_H
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>

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
                set_narServerPort(12345);
                std::fstream fs;
                fs.open ("/home/doguhan/.config/nar/config", std::fstream::in | std::fstream::out | std::fstream::app);
                int flag1 = 0;
                int flag2 = 0;
                if(fs.is_open()) {
                    std::string temp;
                    while( !(fs.eof()) ) {

                        getline(fs,temp);

                        if(temp[0] == 'U'){ //Username
                            flag1 = 1;
                            username = temp.substr(11,temp.size()-11);
                            std::cout << "Username : " << username << std::endl;
                        }
                        else if(temp[0] == 'S'){ //ServerIp
                            flag2 = 1;
                            narServerIP = temp.substr(11,temp.size()-11);
                            std::cout << "ServerIp : " << narServerIP << std::endl;
                        }
                        else{
                            break;
                        }
                    }
                    fs.close();
                    if(flag1 == 1 and flag2 ==1){
                        std::cout << "You are already registered bro! (north remembers...)" << std::endl;
                    }
                    else { //already registered
                        fs.open ("/home/doguhan/.config/nar/config", std::fstream::in | std::fstream::out | std::fstream::app);
                        std::cout << "Username : ";
                        std::cin >> username;
                        fs << "Username : ";
                        fs << username;
                        fs << '\n';
                        std::cout << "ServerIp : ";
                        std::cin >> narServerIP;
                        fs << "ServerIp : ";
                        fs << narServerIP;
                        fs << '\n';
                        std::cout << "Thank you! It's done." << std::endl;
                        fs.close();
                    }
                }
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
