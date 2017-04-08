#ifndef NAR_SERVER_PEERS_H
#define NAR_SERVER_PEERS_H
#include <mutex>
#include <string>
#include <iostream>
#include "sockinfo.h"
#include "Database.h"
#include "dbstructs.h"
#include <random>
using std::string;
namespace nar {
    class Peers {
        private:
            std::mutex read_mtx;
            std::mutex write_mtx;
            std::map<string, nar::SockInfo*> _keepalives;
            std::vector<string> _macs;
            nar::Database * _db;


            int read_count = 0;
            void read_start();
            void read_end();
            void write_start();
            void write_end();
        public:
            Peers(nar::Database * db): _db(db) {}
            void insert_keepalive(string& mac_id, nar::Socket* skt);
            void delete_keepalive(string& mac_id);
            nar::SockInfo* peer_select(nar::DBStructs::User& user, unsigned long int chunk_size);
            nar::SockInfo* random_policy(nar::DBStructs::User& user, unsigned long chunk_size);
    };
}


#endif
