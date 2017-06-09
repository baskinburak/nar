//
// Created by fatih on 09.06.2017.
//
#include <mutex>
#include <nar/narserver/Database.h>
#ifndef NARSERVER_DATABASEREADLOCK_H
#define NARSERVER_DATABASEREADLOCK_H
namespace nar{
    class DatabaseReadLock {
        private:

            nar::Database *_db;
        public:
            DatabaseReadLock(nar::Database * db){
                this->_db = db;
                this->_db->read_start();
            }
            ~DatabaseReadLock(){
                this->_db->read_end();
            }

    };
}

#endif //NARSERVER_DATABASEREADLOCK_H
