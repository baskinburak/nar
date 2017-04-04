#ifndef NAR_DATABASE_H
#define NAR_DATABASE_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <mysql_driver.h>
#include <nar/narserver/dbstructs.h>
#include <nar/narserver/spestruct.h>




namespace nar {


    class Database {
        private:
            std::string _dbname;
            std::string _user;
            std::string _pass;
            sql::Connection * _con;
            sql::Driver * _driver;

            nar::db::User turnUser(nar::User & user);
            nar::db::File turnFile(nar::File & file);
            nar::db::Chunk turnChunk(nar::Chunk & chunk);
            nar::db::UserToFile turnUserToFile(nar::UserToFile & userToFile);
            nar::db::ChunkToMachine turnChunkToMachine(nar::ChunkToMachine & chunkToMachine);
            nar::db::Machine turnMachine(nar::Machine & machine);
            nar::db::Directory turnDirectory(nar::Directory & directory);
            nar::db::DirectoryTo turnDirectoryTo(nar::DirectoryTo & directoryTo);

        public:
            Database()=default;
            ~Database();


            void setUser(std::string user);
            void setPass(std::string pass);
            void setDbname(std::string dbname);
            void connect();
            std::string getUserNameOfDatabase();
            std::string getPass();
            std::string getDbname();
            void insertDirectory(struct Directory & directory);
            void insertUser(struct User & user);
            void insertChunk(struct Chunk & chunk);
            void insertFile(struct File & file);
            void insertMachine(struct Machine & machine);
            void insertDirectoryTo(struct DirectoryTo & directoryTo);
            void insertUserToFile(struct UserToFile & userToFile);
            void insertChunkToMachine(struct ChunkToMachine & chunkToMachine);

            void updateUserName(struct User & user);
            void updateUserAESCrypted(struct User & user);
            void updateUserRSAPriCrypted(struct User & user);
            void updateUserRSAPub(struct User & user);
            void updateUser(struct User & user);
            void updateDirectory(struct Directory & directory);
            void updateDirectoryName(struct Directory & directory);
            void updateDirectorySize(struct Directory & directory);
            void updateChunkFile(struct Chunk & chunk);
            void updateChunkSize(struct Chunk & chunk);
            void updateChunk(struct Chunk & chunk);
            void updateFile(struct File & file);
            void updateFileName(struct File & file);
            void updateFileSize(struct File & file);
            void updateMachine(struct Machine & machine);
            void updateMachineQuota(struct Machine & machine);
            void updateMachineDiskSpace(struct Machine & machine);
            void updateMachineUserId(struct Machine & machine);

            void deleteUser(struct User & user);
            void deleteChunk(struct Chunk & chunk);
            void deleteFile(struct File & file);
            void deleteMachine(struct Machine & machine);
            void deleteUserToFile(struct UserToFile & userToFiles);
            void deleteChunkToMachine(struct ChunkToMachine & chunkToMachine);
            void deleteDirectoryTo(struct DirectoryTo & directoryTo);
            void deleteDirectory(struct Directory & directory);

            struct Directory getDirectory(long long int dirId);
            std::vector<nar::File> getDirectoryFile(long long int dirId);
            std::vector<nar::Directory> getDirectoryDir(long long int dirId);
            std::vector<nar::User> getUserFromFile(long long int fileId);
            struct User getUser(std::string name);
            struct User getUser(long long int userId);
            struct Machine getMachine(std::string machine_id);
            std::vector<struct File> getUserFiles(long long int user_id);
            struct File getFile(long long int file_id);
            std::vector<struct Chunk> getChunks(long long int file_id);
            struct Chunk getChunk(long long int  chunk_id);
            std::vector<struct Machine> getMachines(long long int chunk_id);


            long long int getNextFileId();
            long long int getNextChunkId();
            long long int getNextDirectoryId();
            struct Directory findDirectoryId(std::string user_name,std::string dir_name);

    };

}
#endif // NAR_DATABASE_H
