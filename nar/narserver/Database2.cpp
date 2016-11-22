#include "Database.h"


void nar::Database::updateUserName(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Users
                                        SET Users.User_name = ?
                                        WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateUserQuota(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Users
                                        SET Users.Quota = ?
                                        WHERE Users.User_id = ?;");
    prep_stmt->setObject(1,user.quota);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserDiskSpace(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Users
                                        SET Users.Disk_space = ?
                                        WHERE Users.User_id = ?;");
    prep_stmt->setObject(1,user.disk_space);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserCryptedKey(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Users
                                        SET Users.CryptedKey = ?
                                        WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.cryptedKey);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUser(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Users
                                        SET Users.User_name= ?,Users.Quota = ? ,Users.Disk_space = ? ,
                                        WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setObject(2,user.quota);
    prep_stmt->setObject(3,user.disk_space);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunk(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.File_id= ?,Chunks.Chunk_size = ?
                                        WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setObject(1,chunk.file_id);
    prep_stmt->setObject(2,chunk.chunk_size);
    prep_stmt->setObject(3,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}


void nar::Database::updateChunkFile(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.File_id= ?
                                        WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setObject(1,chunk.file_id);
    prep_stmt->setObject(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunkSize(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.Chunk_size = ?
                                        WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setObject(1,chunk.chunk_size);
    prep_stmt->setObject(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFile(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Files
                                        SET Files.File_name = ?,
                                            Files.File_size = ?,
                                            Files.File_type = ?
                                        WHERE Files.File_id= ?;");
    prep_stmt->setString(1,file.file_name);
    prep_stmt->setObject(2,file.file_size);
    prep_stmt->setString(3,file.file_type);
    prep_stmt->setObject(4,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFileName(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Files
                                        SET Files.File_name = ?
                                        WHERE Files.File_id= ?;");
    prep_stmt->setString(1,file.file_name);
    prep_stmt->setObject(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFileSize(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Files
                                        SET Files.File_size = ?,
                                        WHERE Files.File_id= ?;");
    prep_stmt->setObject(1,file.file_size);
    prep_stmt->setObject(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateFileType(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Files
                                        SET Files.File_type = ?
                                        WHERE Files.File_id= ?;");

    prep_stmt->setString(1,file.file_type);
    prep_stmt->setObject(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachine(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Machines
                                        SET Machines.Machine_quota = ? ,
                                            Machines.Machine_diskSpace ?
                                        WHERE Machines.Machine_id= ?;");
    prep_stmt->setObject(1,machine.machine_quota);
    prep_stmt->setObject(2,machine.machine_diskSpace);
    prep_stmt->setString(3,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachineQuota(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Machines
                                        SET Machines.Machine_quota = ?
                                        WHERE Machines.Machine_id= ?;");
    prep_stmt->setObject(1,machine.machine_quota);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachineDiskSpace(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("UPDATE Machines
                                        SET Machines.Machine_diskSpace ?
                                        WHERE Machines.Machine_id= ?;");
    prep_stmt->setObject(1,machine.machine_diskSpace);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUser(struct User & user){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM Users
                                        WHERE Users.User_id = ? ;");
    prep_stmt->setObject(1,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunk(struct Chunk & chunk){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM Chunks
                                        WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setObject(1,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteFile(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM Files
                                        WHERE Files.File_id= ?;");
    prep_stmt->setObject(1,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteMachine(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM Machines
                                        WHERE Machines.Machine_id= ?;");
    prep_stmt->setString(1,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUserToFile(struct UserToFiles & userToFiles){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM UserToFile
                                        WHERE UserToFile.User_id= ? AND UserToFile.File_id = ?;");
    prep_stmt->setObject(1,userToFiles.user_id);
    prep_stmt->setObject(2,userToFiles.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunkToMachine(struct ChunkToMachine & chunkToMachine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = con->prepareStatement("DELETE FROM ChunkToMachine
                                        WHERE ChunkToMachine.Chunk_id= ? AND ChunkToMachine.Machine_id = ?;");
    prep_stmt->setObject(1,chunkToMachine.chunk_id);
    prep_stmt->setString(2,chunkToMachine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
std::vector<struct File> getUserFiles(long long int user_id){
    std::vector<struct File> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = con->prepareStatement("SELECT File_id, File_name, File_size, File_type
                                        From Files
                                        Where Files.File_id IN (Select File_id
                                                                From UserToFiles
                                                                Where UserToFiles.User_id = ?);");
    prep_stmt->setObject(1,user_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        struct File a;
        a.file_id = res->getObject("File_id");
        a.file_name = res->getString("File_name");
        a.file_size = res->getObject("File_size");
        a.file_type = res->getString("File_type");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
std::vector<struct Chunks> getChunk(long long int file_id){
    std::vector<struct Chunks> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = con->prepareStatement("SELECT Chunk_id, File_id, Chunk_size
                                        From Chunks
                                        Where Chunks.File_id = ?;");
    prep_stmt->setObject(1,file_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        struct Chunks a;
        a.chunk_id = res->getObject("Chunk_id");
        a.file_id = res->getObject("File_id");
        a.chunk_size = res->getObject("Chunk_size");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
std::vector<struct Machine> getMachine(long long int chunk_id){
    std::vector<struct Machine> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = con->prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace
                                        From Machines
                                        Where Machines.Machine_id IN (Select Machine_id
                                                                From ChunkToMachine
                                                                Where ChunkToMachine.Chunk_id = ?);");
    prep_stmt->setObject(1,chunk_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        struct Machine a;
        a.machine_id = res->getString("File_id");
        a.user_id = res->getObject("File_name");
        a.machine_quota = res->getObject("File_size");
        a.Machine_diskSpace = res->getObject("File_type");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
