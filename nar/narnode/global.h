#ifndef NAR_GLOBAL_H
#define NAR_GLOBAL_H
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <nar/narnode/utility.h>
#include <boost/asio.hpp>
#include <cstdlib>
#include <nar/lib/Logging/logger.h>
#include <atomic>

extern nar::Logger* nar_log;
#define NAR_LOG *nar_log                            // BURAYA BAKARLAR

namespace nar {
    class Global {
        private:
			std::string _server_ip;
			unsigned short _server_port;
            std::string _nar_folder;
            std::string _file_folder;
			std::string _machine_id;
            boost::asio::io_service _ioserv;
            std::fstream _config_file;

            boost::asio::ssl::context _srv_ctx;
            boost::asio::ssl::context _ipc_ctx;

            std::recursive_mutex read_mtx;
            std::recursive_mutex write_mtx;
            int read_count = 0;
            void read_start();
            void read_end();
            void write_start();
            void write_end();

            std::string _config_path;
            std::recursive_mutex _file_mtx;
            void write_config();
        public:
            Global(std::string config_path);
            ~Global();
			std::string get_server_ip();
			unsigned short get_server_port();
			std::string get_machine_id();
            std::string get_nar_folder();
            std::string get_file_folder();
            boost::asio::io_service& get_ioserv();
			void set_server_ip(const std::string& ip);
			void set_server_port(unsigned short port);
            void set_nar_folder(std::string& fold);
            void set_file_folder(std::string& fold);
			void set_machine_id(std::string& id);
            nar::Socket* establish_server_connection();
            std::string machine_register();
            boost::asio::ssl::context& get_ipc_ctx();
            boost::asio::ssl::context& get_srv_ctx();
    };
}

#endif
