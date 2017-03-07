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
            nar::Database* _db;
            int read_count = 0;
            void read_start();
            void read_end();
            void write_start();
            void write_end();

        public:
            map<string, nar::SockInfo*> keepalives;
            std::atomic<unsigned short> randevous_port;
            std::atomic<unsigned short> stream_id;
            ServerGlobal();
            ServerGlobal(string db_user, string db_pass, string db_name);
            string get_db_name();
            string get_db_user();
            string get_db_pass();
            void set_db_name(string db_name);
            void set_db_user(string db_user);
            void set_db_pass(string db_pass);
            void connect_db();
            nar::Database* get_db();
    };
}

#endif
