#ifndef NAR_GLOBAL_H
#define NAR_GLOBAL_H
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
#include <boost/asio.hpp>
#include <cstdlib>
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
			std::string narMacId;
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
				std::cout << "Here1" << std::endl;
                std::string temps(getenv("HOME"));
                temps += std::string("/.config/nar/config");
                fs.open (temps.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
                std::cout << "Here1" << std::endl;
                int flag1 = 0;
                int flag2 = 0;
				int flag3 = 0;
                if(fs.is_open()) {
                    std::string temp;
                    while( !(fs.eof()) ) {

                        getline(fs,temp);

                        if(temp[0] == 'U'){ //Username
                            flag1 = 1;
                            username = temp.substr(11,temp.size()-11);
                            std::cout << "Username : " << username << std::endl;
                        }
                        else if(temp[0] == 'S') { //ServerIp :
                            flag2 = 1;
                            narServerIP = temp.substr(11,temp.size()-11);
                            std::cout << "ServerIp : " << narServerIP << std::endl;
                        }
						else if(temp[0] == 'M') { // MachinId
							flag3 = 1;
							narMacId = temp.substr(11, temp.size()-11);
							std::cout << "Machine Id : " << narMacId << std::endl;
						}
                        else{
                            break;
                        }
                    }
                    fs.close();
					/*if(flag3 == 0) {		// First time, register machine
						nar::Socket serverSck;
						std::cout << "Here2" << std::endl;
						serverSck.create();
						serverSck.connect(narServerIP,narServerPort);
						nar::send_message(serverSck, std::string("{\"header\":{\"action\": \"machine_register\",\"channel\": \"ps\"}}"));
						std::string message = nar::get_message(serverSck);
						std::cout << message << std::endl;
						nlohmann::json serverReq;
						serverReq = nlohmann::json::parse(message);
						narMacId = serverReq["payload"]["machine_id"];
					} */
                    if(flag1 == 1 and flag2 ==1 and flag3 == 1){ //already registered
                        std::cout << "You are already registered bro! (north remembers...)" << std::endl;
                    }
                    else {
                        std::string temps(getenv("HOME"));
                        temps += std::string("/.config/nar/config");
                        fs.open (temps.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
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
						nar::Socket serverSck;
						std::cout << "Here2" << std::endl;
						serverSck.create();
						serverSck.connect(narServerIP,narServerPort);
						nar::send_message(serverSck, std::string("{\"header\":{\"action\": \"machine_register\",\"channel\": \"ps\"}}"));
						std::string message = nar::get_message(serverSck);
						std::cout << message << std::endl;
						nlohmann::json serverReq;
						serverReq = nlohmann::json::parse(message);
						narMacId = serverReq["payload"]["machine_id"];

						fs << "MachinId : ";
						fs << narMacId;
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
			std::string get_macId();
			void set_macId(std::string id);
            std::string get_narFolder();
            nar::Socket* establishServerConnection();
            boost::asio::io_service io_serv;
    };
}

#endif
