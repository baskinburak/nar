#include "Database.h"


void nar::Database::updateUserName(User & user) {

    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE User SET User.User_name = ? WHERE User.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateUserQuota(User & user) {

    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE User SET User.Quota = ? WHERE User.User_id = ?;");
    prep_stmt->setObject(1,user.quota);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserDiskSpace(User & user) {

    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE User SET User.Disk_space = ? WHERE User.User_id = ?;");
    prep_stmt->setObject(1,user.disk_space);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserCryptedKey(User & user) {

    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE
                                        User SET User.CryptedKey = ?
                                        WHERE User.User_id = ?;");
    prep_stmt->setString(1,user.cryptedKey);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUser(User & user) {

    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE User
                                        SET User.User_name= ?,User.Quota = ? ,User.Disk_space = ? ,
                                        WHERE User.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setObject(2,user.quota);
    prep_stmt->setObject(3,user.disk_space);
    prep_stmt->setObject(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunk(Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.File_id= ?,Chunks.Chunk_size = ?
                                        WHERE Chunks.chunk_id= ?;");
    prep_stmt->setObject(1,chunk.file_id);
    prep_stmt->setObject(2,chunk.chunk_size);
    prep_stmt->setObject(3,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}


void nar::Database::updateChunkFile(Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.File_id= ?
                                        WHERE Chunks.chunk_id= ?;");
    prep_stmt->setObject(1,chunk.file_id);
    prep_stmt->setObject(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunkSize(Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt
    prep_stmt = con->prepareStatement("UPDATE Chunks
                                        SET Chunks.Chunk_size = ?
                                        WHERE Chunks.chunk_id= ?;");
    prep_stmt->setObject(1,chunk.chunk_size);
    prep_stmt->setObject(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
