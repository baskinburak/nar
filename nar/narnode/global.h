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
#include <atomic>

namespace nar {
    class Global {
        private:
			std::atomic<std::string> _server_ip;
			std::atomic<unsigned short> _server_port;
            std::atomic<std::string> _nar_folder;
            std::atomic<std::string> _file_folder;
			std::atomic<std::string> _machine_id;
            std::atomic<boost::asio::io_service> _ioserv;
            std::fstream _config_file;

            std::string _config_path;
            std::mutex file_mtx;
            void write_config();
        public:
            Global(std::string& config_path);
            ~Global();
			std::atomic<std::string>& get_server_ip();
			std::atomic<unsigned short>& get_server_port();
			std::atomic<std::string>& get_machine_id();
            std::atomic<std::string>& get_nar_folder();
            std::atomic<std::string>& get_file_folder();
            std::atomic<boost::asio::io_service>& get_ioserv();
			void set_server_ip(const std::string& ip);
			void set_server_port(unsigned short port);
            void set_nar_folder(std::string& fold);
            void set_file_folder(std::string& fold);
			void set_machine_id(std::string& id);
            nar::Socket* establish_server_connection();
    };
}

#endif
