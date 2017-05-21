#ifndef NAR_SERVERGLOBAL_H
#define NAR_SERVERGLOBAL_H
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Database.h"
#include "sockinfo.h"
#include <map>
#include <atomic>
#include <set>
#include <nar/narserver/peers.h>
#include <nar/lib/Logging/logger.h>

extern nar::Logger* nar_log;
#define CHUNK_SIZE 1000000                           // BURAYA BAKARLAR
#define NAR_LOG *nar_log                            // BURAYA BAKARLAR


using std::string;
using std::map;
namespace nar {
    /*
     * class that holds the global variables of the server
     *
     * @author: Fatih
     * @privar: _db_name, string, database name
     * @privar: _db_user, string, database user name
     * @privar: _db_pass, string, database password
     * @privar: _db, nar::Database, database connection
     * @privar: read_count, int, for mutex
     * @pubvar: keepalives, map<std::string, nar::SockInfo*>, alive kept sockets
     * @pubvar: io_service, boost::asio::io_service, io_service structure of boost
     * @tested: no
     * @todo: everything
    */
    class ServerGlobal {
        private:
            std::mutex read_mtx;
            std::mutex write_mtx;
            string _db_name;
            string _db_pass;
            string _db_user;
            unsigned short _randezvous_port;
            nar::Database* _db;
            boost::asio::io_service io_service;
            boost::asio::ssl::context ctx;

            unsigned int _next_stream_id;
            int read_count = 0;
            void read_start();
            void read_end();
            void write_start();
            void write_end();

        public:
            ServerGlobal();
            ServerGlobal(std::string db_name, std::string db_user, std::string db_pass);
            nar::Peers* peers;
            string get_db_name();
            string get_db_user();
            string get_db_pass();
            unsigned short get_randezvous_port();
            unsigned int get_next_stream_id();
            void set_db_name(string db_name);
            void set_db_user(string db_user);
            void set_db_pass(string db_pass);
            void set_randezvous_port(unsigned short port);
            void set_next_stream_id(unsigned int);
            void connect_db();
            boost::asio::io_service& get_ioserv();
            boost::asio::ssl::context& get_ctx();
            nar::Database* get_db();
    };
}

#endif
