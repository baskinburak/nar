#include "Database.h"
Database::Database(std::string user, std::string pass, std::string dbname)
{
    _user = user;
    _pass = pass;
    _dbname = dbname;
}

Database::~Database()
{
    delete _con;
    delete _driver;
}

void Database::connect()
{
    _driver = get_driver_instance();
    _con = _driver -> connect("tcp://127.0.0.1:3306", _user.c_str(), _pass.c_str());
    _con -> setSchema(_dbname.c_str());
}

void Database::setUser(std::string user)
{
    _user = user;
}

void Database::setPass(std::string pass)
{
    _pass = pass;
}

void Database::setDbname(std::string dbname)
{
    _dbname = dbname;
}

std::string Database::getUser()
{
    return _user;
}

std::string Database::getPass()
{
    return _pass;
}

std::string Database::getDbname()
{
    return _dbname;
}

void Database::insertUser(struct User &user)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO Users(User_id, User_name,
         Quota, Disk_space, CryptedKey)
            VALUES(?, ?, ?, ?, ?, ?);");
    prep_stmt -> setObject(1, user.user_id);
    prep_stmt -> setString(2, user.user_name);
    prep_stmt -> setObject(3, user.quota);
    prep_stmt -> setObject(4, user.disk_space);
    prep_stmt -> setString(5, user.cryptedKey);

    prep_stmt -> execute();

    delete prep_stmt;
}

void Database::insertChunk(struct Chunk &chunk)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO Chunks(Chunk_id, File_id, Chunk_size)
    VALUES(?, ?, ?);");
    prep_stmt -> setObject(1, chunk.chunk_id);
    prep_stmt -> setObject(2, chunk.file_id);
    prep_stmt -> setString(3, chunk.chunk_size);

    prep_stmt -> execute();

    delete prep_stmt;
}

void Database::insertFile(struct File &file)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO Files(File_id, File_name, File_size, File_type)
    VALUES(?, ?, ?, ?);");
    prep_stmt -> setObject(1, file.file_id);
    prep_stmt -> setString(2, file.file_name);
    prep_stmt -> setString(3, file.file_size);
    prep_stmt -> setString(4, file.file_type);

    prep_stmt -> execute();

    delete prep_stmt;
}

void Database::insertMachine(struct Machine &machine)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO Machines(Machine_id, User_id, Machine_quota, Machine_diskSpace)
    VALUES(?, ?, ?, ?);");
    prep_stmt -> setString(1, machine.machine_id);
    prep_stmt -> setObject(2, machine.user_id);
    prep_stmt -> setObject(3, machine.machine_quota);
    prep_stmt -> setObject(4, machine.machine_diskspace);

    prep_stmt -> execute();

    delete prep_stmt;
}

void Database::insertUserToFile(struct UserToFile &usertofile)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO UserToFile(User_id, File_id)
    VALUES(?, ?);");
    prep_stmt -> setObject(1, usertofile.user_id);
    prep_stmt -> setObject(2, usertofile.file_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

void Database::insertMachine(struct ChunkToPeer &chunktopeer)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("INSERT INTO ChunkToMachine(Chunk_id, Machine_id)
    VALUES(?, ?);");
    prep_stmt -> setObject(1, chunktopeer.chunk_id);
    prep_stmt -> setString(2, chunktopeer.machine_id);

    prep_stmt -> execute();

    delete prep_stmt;
}

struct User Database::getUser(std::string name)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Users
    WHERE Users.User_name == ?;");

    prep_stmt -> setString(1, name);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
}

struct User Database::getUser(long long int user_id)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Users
    WHERE Users.User_id == ?;");

    prep_stmt -> setObject(1, user_id);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
}

struct Machine Database::getMachine(long long int user_id)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Machines
    WHERE Machines.User_id == ?;");

    prep_stmt -> setObject(1, user_id);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
}

struct Machine Database::getMachine(std::string machine_id)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Machines
    WHERE Machines.Machine_id == ?;");

    prep_stmt -> setString(1, machine_id);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
}

struct File Database::getFile(long long int file_id)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Files
    WHERE Files.File_id == ?;");

    prep_stmt -> setObject(1, file_id);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
}

struct Chunk Database::getChunk(long long int chunk_id)
{
    sql::PreparedStatement *prep_stmt;
    prep_stmt = con -> prepareStatement("SELECT *
    FROM Chunks
    WHERE Chunks.Chunk_id == ?;");

    prep_stmt -> setObject(1, chunk_id);

    sql::ResultSet res = prep_stmt -> executeQuery();

    delete prep_stmt;

    return res;
>>>>>>> origin/dogu
}
