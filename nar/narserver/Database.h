#ifndef NAR_DATABASE_H
#define NAR_DATABASE_H

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <set>
#include <mutex>
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
            std::recursive_mutex read_mtx;
            std::recursive_mutex write_mtx;


            int read_count = 0;


            nar::db::User turnUser(nar::DBStructs::User & user);
            nar::db::File turnFile(nar::DBStructs::File & file);
            nar::db::Chunk turnChunk(nar::DBStructs::Chunk & chunk);
            nar::db::UserToFile turnUserToFile(nar::DBStructs::UserToFile & userToFile);
            nar::db::ChunkToMachine turnChunkToMachine(nar::DBStructs::ChunkToMachine & chunkToMachine);
            nar::db::Machine turnMachine(nar::DBStructs::Machine & machine);
            nar::db::Directory turnDirectory(nar::DBStructs::Directory & directory);
            nar::db::DirectoryTo turnDirectoryTo(nar::DBStructs::DirectoryTo & directoryTo);
            nar::db::Session turnSession(nar::DBStructs::Session & session);
            nar::db::TimeTable turnTimeTable(nar::DBStructs::TimeTable & timetable);


        public:
            Database():read_mtx(), write_mtx() {}
            ~Database();

            void read_start();
            void read_end();
            void write_start();
            void write_end();

            void setUser(std::string user);
            void setPass(std::string pass);
            void setDbname(std::string dbname);
            void connect();
            std::string getUserNameOfDatabase();
            std::string getPass();
            std::string getDbname();
            void insertDirectory(struct DBStructs::Directory & directory);
            void insertUser(struct DBStructs::User & user);
            void insertChunk(struct DBStructs::Chunk & chunk);
            void insertFile(struct DBStructs::File & file);
            void insertMachine(struct DBStructs::Machine & machine);
            void insertDirectoryTo(struct DBStructs::DirectoryTo & directoryTo);
            void insertUserToFile(struct DBStructs::UserToFile & userToFile);
            void insertChunkToMachine(struct DBStructs::ChunkToMachine & chunkToMachine);

            unsigned long insertSession(struct DBStructs::Session & session);
            void leaveSession(struct DBStructs::Session& session);

            void updateUserName(struct DBStructs::User & user);
            void updateUserAESCrypted(struct DBStructs::User & user);
            void updateUserRSAPriCrypted(struct DBStructs::User & user);
            void updateUserRSAPub(struct DBStructs::User & user);
            void updateUser(struct DBStructs::User & user);
            void updateDirectory(struct DBStructs::Directory & directory);
            void updateDirectoryName(struct DBStructs::Directory & directory);
            void updateDirectorySize(struct DBStructs::Directory & directory);
            void updateChunkFile(struct DBStructs::Chunk & chunk);
            void updateChunkSize(struct DBStructs::Chunk & chunk);
            void updateChunkHashed(struct DBStructs::Chunk & chunk);
            void updateChunk(struct DBStructs::Chunk & chunk);
            void updateFile(struct DBStructs::File & file);
            void updateFileName(struct DBStructs::File & file);
            void updateFileSize(struct DBStructs::File & file);
            void updateMachine(struct DBStructs::Machine & machine);
            void updateMachineQuota(struct DBStructs::Machine & machine);
            void updateMachineDiskSpace(struct DBStructs::Machine & machine);
            void updateMachineUserId(struct DBStructs::Machine & machine);
            void updateMachineDeleteList(struct DBStructs::Machine & machine);
            void updateTimeTable(db::Session &session);

            void deleteUser(struct DBStructs::User & user);
            void deleteChunk(struct DBStructs::Chunk & chunk);
            void deleteFile(struct DBStructs::File & file);
            void deleteMachine(struct DBStructs::Machine & machine);
            void deleteUserToFile(struct DBStructs::UserToFile & userToFiles);
            void deleteChunkToMachine(struct DBStructs::ChunkToMachine & chunkToMachine);
            void deleteDirectoryTo(struct DBStructs::DirectoryTo & directoryTo);
            void deleteDirectory(struct DBStructs::Directory & directory);

            struct DBStructs::Directory getDirectory(long long int dirId);
            std::vector<nar::DBStructs::File> getDirectoryFile(long long int dirId);
            std::vector<nar::DBStructs::Directory> getDirectoryDir(long long int dirId);
            std::vector<nar::DBStructs::User> getUserFromFile(long long int fileId);
            struct DBStructs::User getUser(std::string name);
            struct DBStructs::User getUser(long long int userId);
            struct DBStructs::Machine getMachine(std::string machine_id);
            std::vector<struct DBStructs::File> getUserFiles(long long int user_id);
            struct DBStructs::File getFile(long long int file_id);
            std::vector<struct DBStructs::Chunk> getChunks(long long int file_id);
            struct DBStructs::Chunk getChunk(long long int  chunk_id);
            std::vector<struct DBStructs::Machine> getMachines(long long int chunk_id);
            std::set<std::string> get_user_machines(nar::DBStructs::User& us);


            long long int getNextFileId(long long int N);
            long long int getNextChunkId(long long int N);
            long long int getNextDirectoryId(long long int N);
            unsigned long getNextSessionId(long long int N);
            struct DBStructs::Directory findDirectoryId(std::string user_name,std::string dir_name);


            bool does_file_exist(std::string& file_name, std::string& parent_name ,std::string& user_name);
            bool does_dir_exist(std::string& dir_name, std::string& parent_name, std::string& user_name);


    };

}
#endif // NAR_DATABASE_H
