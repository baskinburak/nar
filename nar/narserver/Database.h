#ifndef NAR_DATABASE_H
#define NAR_DATABASE_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <nar/dbstructs.h>


using namespace nar;
namespace nar {


    class Database {
        private:
            std::string _dbname;
            std::string _user;
            std::string _pass;
            sql::Connection * _con;
            sql::Driver * _driver;
        public:
            Database(std::string user,std::string pass,std::string dbname);
            ~Database();


            void setUser();
            void setPass();
            void setDbname();
            std::string getUser();
            std::string getPass();
            std::string getDbname();
            void insertUser(User & user);
            void insertChunk(Chunk & chunk);
            void insertFile(File & file);
            void insertMachine(Machine & machine);
            void insertUserToFile(UserToFile & usertofile);
            void insertChunkToPeer(ChunkToPeer & chunktopeer);

            void updateUserName(User & user);
            void updateUserQuota(User & user);
            void updateUserDiskSpace(User & user);
            void updateUserQuota(User & user);
            void updateUser(User & user);
            void updateChunkFile(Chunk & chunk);
            void updateChunkSize(Chunk & chunk);
            void updateChunk(Chunk & chunk);
            void updateFile(File & file);
            void updateMachine();
            void updateUserToFile();
            void updateChunkToPeer();

            void deleteUser();
            void deleteChunk();
            void deleteFile();
            void deleteMachine();
            void deleteUserToFile();
            void deleteChunkToPeer();

            User getUserInfo(std::string name);
            User getUserInfo(long long int user_id);
            Machine getMachine(long long int user_id);
            Machine getMachine(std::string machine_id);
            UserToFiles getUserFiles(long long int user_id);
            File getFile(long long int file_id);
            Chunk getChunk(long long int file_id);
            Chunk getChunk(long long int chunk_id);
            ChunkToPeer getPeer(long long int chunk_id);


    };

}
#endif // NAR_DATABASE_H
