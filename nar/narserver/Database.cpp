#include <nar/narserver/Database.h>


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




nar::db::User nar::Database::turnUser(nar::User & user){
    nar::db::User uk;
    uk.user_id = std::to_string(user.user_id);
    uk.user_name = user.user_name;
    uk.quota = std::to_string(user.quota);
    uk.disk_space = std::to_string(user.disk_space);
    uk.cryptedKey = user.cryptedKey;
    uk.dir_id = std::to_string(user.dir_id);
    return uk;
}
nar::db::File nar::Database::turnFile(nar::File & file){
    nar::db::File fk;
    fk.file_id = std::to_string(file.file_id);
    fk.file_name = file.file_name;
    fk.file_size = std::to_string(file.file_size);
    fk.file_type = file.file_type;
    return fk;
}
nar::db::Chunk nar::Database::turnChunk(nar::Chunk & chunk){
    nar::db::Chunk ck;
    ck.chunk_id = std::to_string(chunk.chunk_id);
    ck.file_id = std::to_string(chunk.file_id);
    ck.chunk_size = std::to_string(chunk.chunk_size);
    return ck;
}
nar::db::UserToFile nar::Database::turnUserToFile(nar::UserToFile & userToFile){
    nar::db::UserToFile usk;
    usk.user_id = std::to_string(userToFile.user_id);
    usk.file_id = std::to_string(userToFile.file_id);
    return usk;
}
nar::db::ChunkToMachine nar::Database::turnChunkToMachine(nar::ChunkToMachine & chunkToMachine){
    nar::db::ChunkToMachine ctm;
    ctm.chunk_id = std::to_string(chunkToMachine.chunk_id);
    ctm.machine_id = chunkToMachine.machine_id;
    return ctm;
}
nar::db::Machine nar::Database::turnMachine(nar::Machine & machine){
    nar::db::Machine mk;
    mk.machine_id = machine.machine_id;
    mk.user_id = std::to_string(machine.user_id);
    mk.machine_quota = std::to_string(machine.machine_quota);
    mk.machine_diskSpace = std::to_string(machine.machine_diskSpace);
    return mk;
}
nar::db::Directory nar::Database::turnDirectory(nar::Directory & directory){
    nar::db::Directory dc;
    dc.dir_id = std::to_string(directory.dir_id);
    dc.dir_name = directory.dir_name;
    dc.dir_size = std::to_string(directory.dir_size);
    dc.change_time = std::to_string(directory.change_time);
    return dc;
}
nar::db::DirectoryTo nar::Database::turnDirectoryTo(nar::DirectoryTo & directoryTo){
    nar::db::DirectoryTo dct;
    dct.dir_id = std::to_string(directoryTo.dir_id);
    dct.item_id = std::to_string(directoryTo.item_id);
    return dct;
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

std::string nar::Database::getUserNameOfDatabase()
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

void nar::Database::insertUser(struct User &us)
{
    nar::Directory d;
    d.dir_name = std::string("/");
    long long int dirId = getNextDirectoryId();
    if (dirId == -1){
        dirId = 1;
    }
    sql::SQLString dir_id = std::to_string(dirId);
    insertDirectory(d);
    nar::db::User user = turnUser(us);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Users(User_name, "
                                         "Quota, Disk_space, CryptedKey,Dir_id ) "
                                            "VALUES(?, ?, ?, ?, ?);");
    prep_stmt -> setString(1, user.user_name);
    prep_stmt -> setBigInt(2, user.quota);
    prep_stmt -> setBigInt(3, user.disk_space);
    prep_stmt -> setString(4, user.cryptedKey);
    prep_stmt -> setBigInt(5, dir_id);

    prep_stmt -> execute();

    delete prep_stmt;
}



void nar::Database::insertDirectory(struct Directory & dir){
    nar::db::Directory directory = turnDirectory(dir);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Directories(Dir_name, "
                                            "Dir_size) "
                                            "VALUES( ?, ?);");
;
    prep_stmt -> setString(1, directory.dir_name);
    prep_stmt -> setBigInt(2, directory.dir_size);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertDirectoryTo(struct DirectoryTo & dirTo){
    nar::db::DirectoryTo directoryTo = turnDirectoryTo(dirTo);
    bool keeper = dirTo.ForD;
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO DirectoryTo( Dir_id, Item_id, "
                                            "ForD ) "
                                            "VALUES(?, ?, ?);");
    prep_stmt -> setBigInt(1,directoryTo.dir_id);
    prep_stmt -> setBigInt(2, directoryTo.item_id);
    prep_stmt -> setBoolean(3, keeper);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertChunk(struct Chunk &ch)
{
    nar::db::Chunk chunk = turnChunk(ch);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Chunks( File_id, Chunk_size) "
                                            "VALUES( ?, ?);");
    prep_stmt -> setBigInt(1, chunk.file_id);
    prep_stmt -> setString(2, chunk.chunk_size);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertFile(struct File &fi)
{
    nar::db::File file = turnFile(fi);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO Files( File_name, File_size, File_type) "
                                            "VALUES( ?, ?, ?);");
    prep_stmt -> setString(1, file.file_name);
    prep_stmt -> setString(2, file.file_size);
    prep_stmt -> setString(3, file.file_type);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertMachine(struct Machine &ma)
{
    nar::db::Machine machine = turnMachine(ma);
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

void nar::Database::insertUserToFile(struct UserToFile &use)
{
    nar::db::UserToFile userToFile = turnUserToFile(use);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO UserToFile(User_id, File_id) "
                                            "VALUES(?, ?);");
    prep_stmt -> setBigInt(1, userToFile.user_id);
    prep_stmt -> setBigInt(2, userToFile.file_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

void nar::Database::insertChunkToMachine(struct ChunkToMachine &chu)
{
    nar::db::ChunkToMachine chunktomachine = turnChunkToMachine(chu);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con -> prepareStatement("INSERT INTO ChunkToMachine(Chunk_id, Machine_id) "
                                            "VALUES(?, ?);");
    prep_stmt -> setBigInt(1, chunktomachine.chunk_id);
    prep_stmt -> setString(2, chunktomachine.machine_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

nar::User nar::Database::getUser(std::string name)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT User_id,User_name,Quota,Disk_space,CryptedKey, "
                                            "Dir_id, UNIX_TIMESTAMP(Change_time) As Time "
                                            "FROM Users "
                                            "WHERE Users.User_name = ?;");

    prep_stmt -> setString(1, name);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    nar::User a;
    a.user_id = -1;
    while(res->next()){
        a.user_id = std::stoll(res->getString("User_id").asStdString());
        a.user_name = res->getString("User_name").asStdString();
        a.quota = std::stoll(res->getString("Quota").asStdString());
        a.disk_space = std::stoll(res->getString("Disk_space").asStdString());
        a.cryptedKey = res->getString("CryptedKey").asStdString();
        a.dir_id = std::stoll(res->getString("Dir_id").asStdString());

        a.change_time = res->getUInt64("Time");
    }
    delete res;
    return a;
}

nar::User nar::Database::getUser(long long int userId)
{
    sql::SQLString user_id = std::to_string(userId);
    sql::PreparedStatement *prep_stmt;
        sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT User_id,User_name,Quota,Disk_space,CryptedKey, "
                                        "Dir_id, UNIX_TIMESTAMP(Change_time) As Time "
                                        "FROM Users "
                                        "WHERE Users.User_id = ?;");

    prep_stmt -> setBigInt(1, user_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    nar::User a;
    a.user_id = -1;
    while(res->next()){
        a.user_id = std::stoll(res->getString("User_id").asStdString());
        a.user_name = res->getString("User_name").asStdString();
        a.quota = std::stoll(res->getString("Quota").asStdString());
        a.disk_space = std::stoll(res->getString("Disk_space").asStdString());
        a.cryptedKey = res->getString("CryptedKey").asStdString();
        a.dir_id = std::stoll(res->getString("Dir_id").asStdString());

        a.change_time = res->getUInt64("Time");
    }
    delete res;
    return a;
}

nar::Machine nar::Database::getMachine(long long int userId)
{
    sql::SQLString user_id = std::to_string(userId);
    sql::PreparedStatement *prep_stmt;
            sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "FROM Machines "
                                        "WHERE Machines.User_id = ?;");

    prep_stmt -> setBigInt(1, user_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    nar::Machine a;
    a.machine_id = std::string("-1");
    while (res->next()) {

        a.machine_id = res->getString("Machine_id").asStdString();
        a.user_id = std::stoll(res->getString("User_id").asStdString());
        a.machine_quota = std::stoll(res->getString("Machine_quota").asStdString());
        a.machine_diskSpace = std::stoll(res->getString("Machine_diskSpace").asStdString());

        a.change_time = res->getUInt64("Time");

    }
    delete res;
    return a;
}

nar::Machine nar::Database::getMachine(std::string machine_id)
{
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "FROM Machines "
                                        "WHERE Machines.Machine_id = ?;");

    prep_stmt -> setString(1, machine_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;

    nar::Machine a;
    a.machine_id = std::string("-1");
    while (res->next()) {

        a.machine_id = res->getString("Machine_id").asStdString();
        a.user_id = std::stoll(res->getString("User_id").asStdString());
        a.machine_quota = std::stoll(res->getString("Machine_quota").asStdString());
        a.machine_diskSpace = std::stoll(res->getString("Machine_diskSpace").asStdString());

        a.change_time = res->getUInt64("Time");

    }
    delete res;
    return a;
}

nar::File nar::Database::getFile(long long int fileId)
{
    sql::SQLString file_id = std::to_string(fileId);
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con -> prepareStatement("SELECT File_id,File_name,File_size,File_type, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "FROM Files "
                                        "WHERE Files.File_id = ?;");

    prep_stmt -> setBigInt(1, file_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    nar::File a;
    a.file_id = -1;
    while (res->next()) {

        a.file_id = std::stoll(res->getString("File_id").asStdString());
        a.file_name = res->getString("File_name").asStdString();
        a.file_size = std::stoll(res->getString("File_size").asStdString());
        a.file_type = res->getString("File_type").asStdString();

        a.change_time = res->getUInt64("Time");

    }
    delete res;
    return a;
}

nar::Chunk nar::Database::getChunk(long long int chunkId)
{
    sql::SQLString chunk_id = std::to_string(chunkId);
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet  *res;
    prep_stmt = _con -> prepareStatement("SELECT Chunk_id,File_id,Chunk_size, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "FROM Chunks"
                                        "WHERE Chunks.Chunk_id = ?;");

    prep_stmt -> setBigInt(1, chunk_id);

    res = prep_stmt -> executeQuery();

    delete prep_stmt;
    struct Chunk a;
    a.chunk_id = -1;
    while (res->next()) {

        a.chunk_id = std::stoll(res->getString("Chunk_id").asStdString());
        a.file_id = std::stoll(res->getString("File_id").asStdString());
        a.chunk_size = std::stoll(res->getString("Chunk_size").asStdString());

        a.change_time = res->getUInt64("Time");

    }
    delete res;
    return a;

}

void nar::Database::updateUserName(struct User & us) {
        nar::db::User user = turnUser(us);

    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.User_name = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.user_name);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateUserQuota(struct User & us) {
    nar::db::User user = turnUser(us);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.Quota = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.quota);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserDiskSpace(struct User & us) {
    nar::db::User user = turnUser(us);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.Disk_space = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.disk_space);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUserCryptedKey(struct User & us) {
    nar::db::User user = turnUser(us);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Users "
                                        "SET Users.CryptedKey = ? "
                                        "WHERE Users.User_id = ?;");
    prep_stmt->setString(1,user.cryptedKey);
    prep_stmt->setBigInt(2,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateUser(struct User & us) {
    nar::db::User user = turnUser(us);
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

void nar::Database::updateChunk(struct Chunk & ch) {
    nar::db::Chunk chunk = turnChunk(ch);
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


void nar::Database::updateChunkFile(struct Chunk & ch) {
    nar::db::Chunk chunk = turnChunk(ch);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Chunks "
                                        "SET Chunks.File_id= ? "
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.file_id);
    prep_stmt->setBigInt(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateChunkSize(struct Chunk & ch) {
    nar::db::Chunk chunk = turnChunk(ch);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Chunks "
                                        "SET Chunks.Chunk_size = ?"
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.chunk_size);
    prep_stmt->setBigInt(2,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFile(struct File & fi){
    nar::db::File file = turnFile(fi);
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
void nar::Database::updateFileName(struct File & fi){
    nar::db::File file = turnFile(fi);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_name = ? "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setString(1,file.file_name);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateFileSize(struct File & fi){
    nar::db::File file = turnFile(fi);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_size = ? "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setBigInt(1,file.file_size);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}

void nar::Database::updateFileType(struct File & fi){
    nar::db::File file = turnFile(fi);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Files "
                                        "SET Files.File_type = ? "
                                        "WHERE Files.File_id= ?;");

    prep_stmt->setString(1,file.file_type);
    prep_stmt->setBigInt(2,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachine(struct Machine & ma){
    nar::db::Machine machine = turnMachine(ma);
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
void nar::Database::updateMachineQuota(struct Machine & ma){
    nar::db::Machine machine = turnMachine(ma);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Machines "
                                        "SET Machines.Machine_quota = ? "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setBigInt(1,machine.machine_quota);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateMachineDiskSpace(struct Machine & ma){
    nar::db::Machine machine = turnMachine(ma);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Machines "
                                        "SET Machines.Machine_diskSpace = ? "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setBigInt(1,machine.machine_diskSpace);
    prep_stmt->setString(2,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateDirectory(struct Directory & dir){
    nar::db::Directory directory = turnDirectory(dir);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Directories "
                                        "SET Dir_name = ?, Dir_size = ? "
                                        "WHERE Dir_id = ?;");
    prep_stmt->setString(1,directory.dir_name);
    prep_stmt->setBigInt(2,directory.dir_size);
    prep_stmt->setBigInt(3,directory.dir_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateDirectoryName(struct Directory & dir){
    nar::db::Directory directory = turnDirectory(dir);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Directories "
                                        "SET Dir_name = ? "
                                        "WHERE Dir_id = ?;");
    prep_stmt->setString(1,directory.dir_name);
    prep_stmt->setBigInt(2,directory.dir_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::updateDirectorySize(struct Directory & dir){
    nar::db::Directory directory = turnDirectory(dir);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con->prepareStatement("UPDATE Directories "
                                        "SET Dir_size = ? "
                                        "WHERE Dir_id = ?;");
    prep_stmt->setBigInt(1,directory.dir_size);
    prep_stmt->setBigInt(2,directory.dir_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUser(struct User & us){
    nar::db::User user = turnUser(us);
    sql::PreparedStatement  *prep_stmt;
    nar::User a = getUser(us.user_name);
    nar::Directory d;
    d.dir_id = a.dir_id;
    deleteDirectory(d);
    prep_stmt = _con->prepareStatement("DELETE FROM Users Where Users.User_id = ?;");
    prep_stmt->setBigInt(1,user.user_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunk(struct Chunk & ch){
    nar::db::Chunk chunk = turnChunk(ch);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Chunks "
                                        "WHERE Chunks.Chunk_id= ?;");
    prep_stmt->setBigInt(1,chunk.chunk_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteFile(struct File & fi){
    nar::db::File file = turnFile(fi);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Files "
                                        "WHERE Files.File_id= ?;");
    prep_stmt->setBigInt(1,file.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteMachine(struct Machine & ma){
    nar::db::Machine machine = turnMachine(ma);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Machines "
                                        "WHERE Machines.Machine_id= ?;");
    prep_stmt->setString(1,machine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteUserToFile(struct UserToFile & use){
    nar::db::UserToFile userToFile = turnUserToFile(use);
    sql::PreparedStatement  *prep_stmt;

    prep_stmt = _con->prepareStatement("DELETE FROM UserToFile "
                                        "WHERE UserToFile.User_id= ? AND UserToFile.File_id = ?;");
    prep_stmt->setBigInt(1,userToFile.user_id);
    prep_stmt->setBigInt(2,userToFile.file_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteChunkToMachine(struct ChunkToMachine & chu){
    nar::db::ChunkToMachine chunkToMachine = turnChunkToMachine(chu);
    sql::PreparedStatement  *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM ChunkToMachine "
                                        "WHERE ChunkToMachine.Chunk_id= ? AND ChunkToMachine.Machine_id = ?;");
    prep_stmt->setBigInt(1,chunkToMachine.chunk_id);
    prep_stmt->setString(2,chunkToMachine.machine_id);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteDirectoryTo(struct DirectoryTo & dirTo){
    nar::db::DirectoryTo directoryTo = turnDirectoryTo(dirTo);
    bool keeper = dirTo.ForD;
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM DirectoryTo "
                                        "WHERE Dir_name = ? AND Item_id = ? AND ForD = ?;");
    prep_stmt->setBigInt(1,directoryTo.dir_id);
    prep_stmt->setBigInt(2,directoryTo.item_id);
    prep_stmt->setBoolean(3,keeper);
    prep_stmt->execute();
    delete prep_stmt;
}
void nar::Database::deleteDirectory(struct Directory & dir){
    nar::db::Directory directory = turnDirectory(dir);
    sql::PreparedStatement *prep_stmt;
    prep_stmt = _con->prepareStatement("DELETE FROM Directories "
                                        "WHERE Dir_id = ?;");
    prep_stmt->setBigInt(1,directory.dir_id);
    prep_stmt->execute();
    delete prep_stmt;
}
nar::Directory nar::Database::getDirectory(long long int dirId){
    sql::SQLString dir_id = std::to_string(dirId);
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT Dir_id, Dir_name, Dir_size, UNIX_TIMESTAMP(Change_time) As Time "
                                        "From Directories "
                                        "WHERE Dir_id = ?");
    prep_stmt->setBigInt(1,dir_id);
    res = prep_stmt->executeQuery();
    nar::Directory a;
    while(res->next()){
        a.dir_id = std::stoll(res->getString("Dir_id").asStdString());
        a.dir_name = res->getString("Dir_name").asStdString();
        a.dir_size = std::stoll(res->getString("Dir_size").asStdString());

        a.change_time = res->getUInt64("Time");
    }

}
std::vector<nar::File> nar::Database::getDirectoryFile(long long int dirId){
    bool keeper = false;
    sql::SQLString dir_id = std::to_string(dirId);
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    std::vector<nar::File> output;
    prep_stmt = _con->prepareStatement("SELECT File_id, File_name,File_size,File_type, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        " FROM Files "
                                        "Where Files.File_id IN (Select Item_id AS File_id "
                                                                "From DirectoryTo "
                                                                "Where Dir_id = ? AND ForD = FALSE);");
    prep_stmt->setBigInt(1,dir_id);
    res = prep_stmt->executeQuery();
    while(res->next()){
        nar::File a;
        a.file_id = std::stoll(res->getString("File_id").asStdString());
        a.file_name = res->getString("File_name").asStdString();
        a.file_size = std::stoll(res->getString("File_size").asStdString());
        a.file_type = res->getString("File_type").asStdString();
        a.change_time = res->getUInt64("Time");

        output.push_back(a);
    }
    delete res;
    delete prep_stmt;
    return output;

}
std::vector<nar::Directory> nar::Database::getDirectoryDir(long long int dirId){
    bool keeper = false;
    sql::SQLString dir_id = std::to_string(dirId);
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    std::vector<nar::Directory> output;
    prep_stmt = _con->prepareStatement("SELECT Dir_id, Dir_name,Dir_size, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "From Directories "
                                        "Where Directories.Dir_id IN (Select Item_id "
                                                                "From DirectoryTo "
                                                                "Where Dir_id = ? AND ForD = TRUE);");
    prep_stmt->setBigInt(1,dir_id);
    res = prep_stmt->executeQuery();
    while(res->next()){
        nar::Directory a;
        a.dir_id = std::stoll(res->getString("Dir_id").asStdString());
        a.dir_name = res->getString("Dir_name").asStdString();
        a.dir_size = std::stoll(res->getString("Dir_size").asStdString());

        a.change_time = res->getUInt64("Time");

        output.push_back(a);
    }
    delete res;
    delete prep_stmt;
    return output;

}
long long int nar::Database::getNextFileId(){
    sql::PreparedStatement *prep_stmt;
    long long int keep = -1;
    sql::ResultSet *res;
    prep_stmt=_con->prepareStatement("Select MAX(File_id)+1 AS f From Files");
    res = prep_stmt->executeQuery();
    while(res->next()){
        if(!res->isNull("f")){
            keep = std::stoll(res->getString("f").asStdString());
        }
    }
    delete res;
    delete prep_stmt;
    return keep;
}
long long int nar::Database::getNextChunkId(){
    sql::PreparedStatement *prep_stmt;
    long long int keep = -1;
    sql::ResultSet *res;
    prep_stmt=_con->prepareStatement("Select MAX(Chunk_id)+1 AS f From Chunks");
    res = prep_stmt->executeQuery();
    while(res->next()){
        if(!res->isNull("f")){
            keep = std::stoll(res->getString("f").asStdString());
        }
    }
    delete res;
    delete prep_stmt;
    return keep;
}
long long int nar::Database::getNextDirectoryId(){
    sql::PreparedStatement *prep_stmt;
    long long int keep = -1;
    sql::ResultSet *res;
    prep_stmt=_con->prepareStatement("Select MAX(Dir_id)+1 AS f From Directories");
    res = prep_stmt->executeQuery();
    while(res->next()){

        if(!res->isNull("f")){
            keep = std::stoll(res->getString("f").asStdString());
        }
    }
    delete res;
    delete prep_stmt;
    return keep;
}
std::vector<nar::File>  nar::Database::getUserFiles(long long int userId){
    sql::SQLString user_id = std::to_string(userId);
    std::vector<nar::File> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT File_id, File_name, File_size, File_type, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "From Files "
                                        "Where Files.File_id IN (Select File_id "
                                                                "From UserToFile "
                                                                "Where UserToFile.User_id = ?);");
    prep_stmt->setBigInt(1,user_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::File a;
        a.file_id = std::stoll(res->getString("File_id").asStdString());
        a.file_name = res->getString("File_name").asStdString();
        a.file_size = std::stoll(res->getString("File_size").asStdString());
        a.file_type = res->getString("File_type").asStdString();

        a.change_time = res->getUInt64("Time");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}

std::vector<nar::Chunk>  nar::Database::getChunks(long long int fileId){
    sql::SQLString file_id = std::to_string(fileId);
    std::vector<nar::Chunk> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT Chunk_id, File_id, Chunk_size, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "From Chunks "
                                        "Where Chunks.File_id = ?;");
    prep_stmt->setBigInt(1,file_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::Chunk a;
        a.chunk_id = std::stoll(res->getString("Chunk_id").asStdString());
        a.file_id = std::stoll(res->getString("File_id").asStdString());
        a.chunk_size = std::stoll(res->getString("Chunk_size").asStdString());

        a.change_time = res->getUInt64("Time");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
std::vector<nar::Machine>  nar::Database::getMachines(long long int chunkId){
    sql::SQLString chunk_id = std::to_string(chunkId);
    std::vector<nar::Machine> output;
    sql::PreparedStatement  *prep_stmt;
    sql::ResultSet *res;
    prep_stmt = _con->prepareStatement("SELECT Machine_id, User_id, Machine_quota, Machine_diskSpace, "
                                        "UNIX_TIMESTAMP(Change_time) As Time "
                                        "From Machines "
                                        "Where Machines.Machine_id IN (Select Machine_id "
                                                                "From ChunkToMachine "
                                                                "Where ChunkToMachine.Chunk_id = ?);");
    prep_stmt->setBigInt(1,chunk_id);
    res = prep_stmt->executeQuery();
    while (res->next()) {
        nar::Machine a;
        a.machine_id = res->getString("Machine_id").asStdString();
        a.user_id = std::stoll(res->getString("User_id").asStdString());
        a.machine_quota = std::stoll(res->getString("Machine_quota").asStdString());
        a.machine_diskSpace = std::stoll(res->getString("Machine_diskSpace").asStdString());

        a.change_time = res->getUInt64("Time");
        output.push_back(a);

    }
    delete prep_stmt;
    delete res;
    return output;
}
nar::Directory nar::Database::findDirectoryId(std::string user_name,std::string dir_name){
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    std::vector<std::string> names;
    std::size_t found;
    std::size_t last_found;
    std::string temp;
    std::string sql_string = "";
    unsigned int count = 1;
    sql::SQLString query;
    if(dir_name[dir_name.length()-1] == '/'){
        dir_name = dir_name.substr(0,dir_name.length()-1);
    }

    //names.push_back(std::string("/"));


    if(dir_name[0] == '/'){
        found = dir_name.find("/",1);
        last_found = 1;
    }
    else{
        found = dir_name.find("/");
        last_found = 0;
    }
    while(found != std::string::npos){
        temp = dir_name.substr(last_found,found-last_found);
        last_found = found+1;
        found = dir_name.find("/",last_found);
        names.push_back(temp);
    }
    temp = dir_name.substr(last_found);
    names.push_back(temp);
    nar::Directory result_dir;
    sql_string+=std::string(" SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN (select Dir_id from Users where user_name= ? )");
    for(int i=0;i<names.size()-1;i++){
            sql_string =std::string(" SELECT Dir_id From Directories WHERE Dir_name = ? AND Dir_id IN ( ")+sql_string +std::string(")");
            sql_string = std::string(" SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN (")+sql_string+std::string(")");
    }
    sql_string =std::string(" SELECT Dir_id, Dir_name, Dir_size, UNIX_TIMESTAMP(Change_time) As Time From Directories WHERE Dir_name = ? AND Dir_id IN ( ")+sql_string +std::string(")");
    sql_string = sql_string +(";");
    query = sql::SQLString(sql_string);
    std::reverse(names.begin(),names.end());
    prep_stmt = _con->prepareStatement(query);
    for(int i= 0;i<names.size();i++){

        prep_stmt->setString(count, sql::SQLString(names[i]));
        count++;
    }
    prep_stmt->setString(count, sql::SQLString(user_name));

    res = prep_stmt->executeQuery();
    while(res->next()){
        result_dir.dir_id = std::stoll(res->getString("Dir_id").asStdString());
        result_dir.dir_name = res->getString("Dir_name").asStdString();
        result_dir.dir_size = std::stoll(res->getString("Dir_size").asStdString());

        result_dir.change_time = res->getUInt64("Time");
    }
    delete prep_stmt;
    delete res;
    return result_dir;


}
