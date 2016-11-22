#include "Database.h"


nar::Database::~Database()
{
    delete _con;
//    delete _driver;
}

void nar::Database::connect()
{
    _driver = get_driver_instance();
    _con = _driver -> connect("tcp://127.0.0.1:3306", _user.c_str(), _pass.c_str());
    _con -> setSchema(_dbname.c_str());
}

void nar::Database::setUser(std::string user)
{
    _user = user;
}

void nar::Database::setPass(std::string pass)
{
    _pass = pass;
}

void nar::Database::setDbname(std::string dbname)
{
    _dbname = dbname;
}

std::string nar::Database::getUser()
{
    return _user;
}

std::string nar::Database::getPass()
{
    return _pass;
}

std::string nar::Database::getDbname()
{
    return _dbname;
}

void nar::Database::insertUser(struct User &user)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Users(User_id, User_name, "
                                         "Quota, Disk_space, CryptedKey) "
                                            "VALUES(?, ?, ?, ?, ?);");
    prep_stmt->setBigInt(1, user.user_id);
    prep_stmt -> setString(2, user.user_name);
    prep_stmt -> setBigInt(3, user.quota);
    prep_stmt -> setBigInt(4, user.disk_space);
    prep_stmt -> setString(5, user.cryptedKey);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertChunk(struct Chunk &chunk)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Chunks(Chunk_id, File_id, Chunk_size) "
                                            "VALUES(?, ?, ?);");
    prep_stmt -> setBigInt(1, chunk.chunk_id);
    prep_stmt -> setBigInt(2, chunk.file_id);
    prep_stmt -> setString(3, chunk.chunk_size);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertFile(struct File &file)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Files(File_id, File_name, File_size, File_type) "
                                            "VALUES(?, ?, ?, ?);");
    prep_stmt -> setBigInt(1, file.file_id);
    prep_stmt -> setString(2, file.file_name);
    prep_stmt -> setString(3, file.file_size);
    prep_stmt -> setString(4, file.file_type);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertMachine(struct Machine &machine)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Machines(Machine_id, User_id, Machine_quota, Machine_diskSpace) "
                                            "VALUES(?, ?, ?, ?);");
    prep_stmt -> setString(1, machine.machine_id);
    prep_stmt -> setBigInt(2, machine.user_id);
    prep_stmt -> setBigInt(3, machine.machine_quota);
    prep_stmt -> setBigInt(4, machine.machine_diskSpace);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertUserToFile(struct UserToFile &userToFile)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO UserToFile(User_id, File_id) "
                                            "VALUES(?, ?);");
    prep_stmt -> setBigInt(1, userToFile.user_id);
    prep_stmt -> setBigInt(2, userToFile.file_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertChunkToMachine(struct ChunkToMachine &chunktopeer)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO ChunkToMachine(Chunk_id, Machine_id) "
                                            "VALUES(?, ?);");
    prep_stmt -> setBigInt(1, chunktopeer.chunk_id);
    prep_stmt -> setString(2, chunktopeer.machine_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

nar::User nar::Database::getUser(std::string name)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT User_id,User_name,Quota,Disk_space,CryptedKey "
                                            "FROM Users "
                                            "WHERE Users.User_name = ?;");

    prep_stmt -> setString(1, name);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    struct User a;
    while(res->next()){
        a.user_id = res->getString("User_id");
        a.user_name = res->getString("User_name");
        a.quota = res->getString("Quota");
        a.disk_space = res->getString("Disk_space");
        a.cryptedKey = res->getString("CryptedKey");
    }
    delete res;
    return a;
}

nar::User nar::Database::getUser(sql::SQLString user_id)
{
    sql::PreparedStatement *prep_stmt;
        sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT User_id,User_name,Quota,Disk_space,CryptedKey "
                                        "FROM Users "
                                        "WHERE Users.User_id = ?;");

    prep_stmt -> setBigInt(1, user_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
        struct User a;
    while(res->next()){
        a.user_id = res->getString("User_id");
        a.user_name = res->getString("User_name");
        a.quota = res->getString("Quota");
        a.disk_space = res->getString("Disk_space");
        a.cryptedKey = res->getString("CryptedKey");
    }
    delete res;
    return a;
}

nar::Machine nar::Database::getMachine(sql::SQLString user_id)
{
    sql::PreparedStatement *prep_stmt;
            sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace "
                                        "FROM Machines "
                                        "WHERE Machines.User_id = ?;");

    prep_stmt -> setBigInt(1, user_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
            struct Machine a;
    while (res->next()) {

        a.machine_id = res->getString("Machine_id");
        a.user_id = res->getString("User_id");
        a.machine_quota = res->getString("Machine_quota");
        a.machine_diskSpace = res->getString("Machine_diskSpace");


    }
    delete res;
    return a;
}

nar::Machine nar::Database::getMachine(std::string machine_id)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace "
                                        "FROM Machines "
                                        "WHERE Machines.Machine_id = ?;");

    prep_stmt -> setString(1, machine_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;

    struct Machine a;
    while (res->next()) {

        a.machine_id = res->getString("Machine_id");
        a.user_id = res->getString("User_id");
        a.machine_quota = res->getString("Machine_quota");
        a.machine_diskSpace = res->getString("Machine_diskSpace");


    }
    delete res;
    return a;
}

nar::File nar::Database::getFile(sql::SQLString file_id)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT File_id,File_name,File_size,File_type "
                                        "FROM Files "
                                        "WHERE Files.File_id = ?;");

    prep_stmt -> setBigInt(1, file_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    struct File a;
    while (res->next()) {

        a.file_id = res->getString("File_id");
        a.file_name = res->getString("File_name");
        a.file_size = res->getString("File_size");
        a.file_type = res->getString("File_type");


    }
    delete res;
    return a;
}

nar::Chunk nar::Database::getChunk(sql::SQLString chunk_id)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = _con -> prepareStatement("SELECT *"
    "FROM Chunks"
    "WHERE Chunks.Chunk_id = ?;");

    prep_stmt -> setBigInt(1, chunk_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    struct Chunk a;
    while (res->next()) {

        a.chunk_id = res->getString("Chunk_id");
        a.file_id = res->getString("File_id");
        a.chunk_size = res->getString("Chunk_size");


    }
    delete res;
    return a;

}

void nar::Database::updateUserName(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.User_name = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateUserQuota(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.Quota = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.quota);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserDiskSpace(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.Disk_space = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.disk_space);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserCryptedKey(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.CryptedKey = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.cryptedKey);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUser(struct User & user) {

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.User_name= ?,Users.Quota = ? ,Users.Disk_space = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setBigInt(2,user.quota);
    prep_stmt->setBigInt(3,user.disk_space);
    prep_stmt->setBigInt(4,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunk(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Chunks "
                                        "SET Chunks.File_id= ?,Chunks.Chunk_size = ? "
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.file_id);
    prep_stmt->setBigInt(2,chunk.chunk_size);
    prep_stmt->setBigInt(3,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}


void nar::Database::updateChunkFile(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Chunks "
                                        "SET Chunks.File_id= ? "
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.file_id);
    prep_stmt->setBigInt(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunkSize(struct Chunk & chunk) {
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Chunks "
                                        "SET Chunks.Chunk_size = ?"
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.chunk_size);
    prep_stmt->setBigInt(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFile(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_name = ?, "
                                            "Files.File_size = ?, "
                                            "Files.File_type = ? "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setString(1,file.file_name);
    prep_stmt->setBigInt(2,file.file_size);
    prep_stmt->setString(3,file.file_type);
    prep_stmt->setBigInt(4,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFileName(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_name = ? "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setString(1,file.file_name);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFileSize(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_size = ? "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setBigInt(1,file.file_size);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateFileType(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_type = ? "
                                        "WHERE Files.File_id= ?;");

    prep_stmt->setString(1,file.file_type);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachine(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Machines "
                                        "SET Machines.Machine_quota = ? , Machines.Machine_diskSpace ? "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setBigInt(1,machine.machine_quota);
    prep_stmt->setBigInt(2,machine.machine_diskSpace);
    prep_stmt->setString(3,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachineQuota(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Machines "
                                        "SET Machines.Machine_quota = ? "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setBigInt(1,machine.machine_quota);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachineDiskSpace(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Machines "
                                        "SET Machines.Machine_diskSpace ? "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setBigInt(1,machine.machine_diskSpace);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUser(struct User & user){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Users Where Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunk(struct Chunk & chunk){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Chunks "
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteFile(struct File & file){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Files "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setBigInt(1,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteMachine(struct Machine & machine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Machines "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setString(1,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUserToFile(struct UserToFile & userToFiles){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM UserToFile "
                                        "WHERE UserToFile.User_id= ? AND UserToFile.File_id = ?;");
    prep_stmt->setBigInt(1,userToFiles.user_id);
    prep_stmt->setBigInt(2,userToFiles.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunkToMachine(struct ChunkToMachine & chunkToMachine){
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM ChunkToMachine "
                                        "WHERE ChunkToMachine.Chunk_id= ? AND ChunkToMachine.Machine_id = ?;");
    prep_stmt->setBigInt(1,chunkToMachine.chunk_id);
    prep_stmt->setString(2,chunkToMachine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
std::vector<nar::File>  nar::Database::getUserFiles(sql::SQLString user_id){
    std::vector<nar::File> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT File_id, File_name, File_size, File_type "
                                        "From Files "
                                        "Where Files.File_id IN (Select File_id "
                                                                "From UserToFile "
                                                                "Where UserToFile.User_id = ?);");
    prep_stmt->setBigInt(1,user_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::File a;
        a.file_id = res->getString("File_id");
        a.file_name = res->getString("File_name");
        a.file_size = res->getString("File_size");
        a.file_type = res->getString("File_type");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}

std::vector<nar::Chunk>  nar::Database::getChunks(sql::SQLString file_id){
    std::vector<nar::Chunk> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT Chunk_id, File_id, Chunk_size "
                                        "From Chunks "
                                        "Where Chunks.File_id = ?;");
    prep_stmt->setBigInt(1,file_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::Chunk a;
        a.chunk_id = res->getString("Chunk_id");
        a.file_id = res->getString("File_id");
        a.chunk_size = res->getString("Chunk_size");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
std::vector<nar::Machine>  nar::Database::getMachines(sql::SQLString chunk_id){
    std::vector<nar::Machine> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace "
                                        "From Machines "
                                        "Where Machines.Machine_id IN (Select Machine_id "
                                                                "From ChunkToMachine "
                                                                "Where ChunkToMachine.Chunk_id = ?);");
    prep_stmt->setBigInt(1,chunk_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::Machine a;
        a.machine_id = res->getString("File_id");
        a.user_id = res->getString("File_name");
        a.machine_quota = res->getString("File_size");
        a.machine_diskSpace = res->getString("File_type");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
