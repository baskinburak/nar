
/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>

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
#include <nar/narserver/Database.h>
using namespace std;
using namespace nar;
int main(void)
{
    Database db;
    db.setUser(string("root"));
    db.setPass(string("123"));
    db.setDbname(string("nar"));
    db.connect();

    struct User user;  //insert user success
    user.user_id = sql::SQLString("1");
    user.user_name = string("test");
    user.quota = sql::SQLString("10000");
    user.disk_space = sql::SQLString("10000");
    user.cryptedKey = string("asdkajsbdkasnd0");
    //db.insertUser(user);
    user.user_name = string("dayim");
    //db.updateUser(user);
    //struct User test = db.getUser(user.user_id);
    //cout<<test.user_id<<" "<<test.user_name<<endl;
    struct Machine machine;
    machine.machine_id = string("test makinasi");
    machine.user_id = sql::SQLString("1");
    machine.machine_quota = sql::SQLString("5000");
    machine.machine_diskSpace = sql::SQLString("5000");
    struct File file;
    file.file_id = sql::SQLString("2");
    file.file_name = string("test file2");
    file.file_size = sql::SQLString("300");
    file.file_type = string("file");
    //db.insertFile(file);
    struct UserToFile utff;
    utff.user_id = user.user_id;
    utff.file_id = file.file_id;
    vector<struct File> vec = db.getUserFiles(user.user_id);
    cout<<vec[0].file_id<<" "<<vec[1].file_id<<endl;
    //db.insertUserToFile(utff);
    //db.insertMachine(machine);
    //db.deleteMachine(machine);
    //db.deleteUser(user);

    return 0;
}
