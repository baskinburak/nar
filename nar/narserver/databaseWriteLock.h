//
// Created by fatih on 09.06.2017.
//
#include <mutex>
#include <nar/narserver/Database.h>
#ifndef NARSERVER_DATABASEWRITELOCK_H
#define NARSERVER_DATABASEWRITELOCK_H

namespace nar {
    class DatabaseWriteLock {
        private:

            nar::Database *_db;
        public:
            DatabaseWriteLock(nar::Database * db){
                this->_db = db;
                this->_db->write_start();
            }
            ~DatabaseWriteLock(){
                this->_db->write_end();
            }

    };
}



#endif //NARSERVER_DATABASEWRITELOCK_H
