
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

    nar::User user;  //insert user success
    nar::Directory dir;
    nar::DirectoryTo dirTo;
    nar::Chunk ch;
    nar::Machine mh;
    nar::File file;
    nar::UserToFile utff;
    nar::ChunkToMachine ctm;
//    user.user_id = 1;
    /*user.user_name = string("test1");
    db.insertUser(user);
    dir.dir_name = string("homy/");
    dir.dir_size = 12345;
    db.insertDirectory(dir);
    cout<<"dir test"<<endl;
    file.file_name = string("bitirmee.txt");
    file.file_size = 122;
    file.file_type = string("file");
    db.insertFile(file);
    cout<<"file test"<<endl;
    ch.file_id = 2;
    ch.chunk_size = 100;
    db.insertChunk(ch);
    cout<<"ch test"<<endl;
    dirTo.dir_id =1;
    dirTo.item_id = 2;
    dirTo.ForD = 0;
    db.insertDirectoryTo(dirTo);
    cout<<"dirTo1 test"<<endl;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);
    cout<<"dirTo2 test"<<endl;
    mh.machine_id = string("test machinee");
    mh.user_id = 2;
    db.insertMachine(mh);
    cout<<"mh test"<<endl;
    utff.user_id = 2;
    utff.file_id = 2;
    db.insertUserToFile(utff);
    cout<<"utff test"<<endl;
    ctm.chunk_id = 1;
    ctm.machine_id = string("test machinee");
    db.insertChunkToMachine(ctm);
    cout<<"ctm test"<<endl;*/
    /*vector<nar::File> a = db.getDirectoryFile(1);
    for(int i= 0;i<a.size();i++){
        cout<<a[i].file_name<<endl;
    }
    vector<nar::Directory> b = db.getDirectoryDir(1);
    for(int i= 0;i<a.size();i++){
        cout<<b[i].dir_name<<endl;
    }*/
    dir.dir_id = 1;
    db.deleteDirectory(dir);
//    user.quota = 10000;
//    user.disk_space = 10000;
//    user.cryptedKey = string("asdkajsbdkasnd0");
    //db.insertUser(user);
//    nar::User test_us =  db.getUser(1);
    //nar::User test_us2 =  db.getUser(string("test"));
    //cout<<"1 "<<test_us.user_id<<" "<<test_us.user_name<<" "<<test_us.dir_id<<endl;
    //cout<<"2 "<<test_us2.user_id<<" "<<test_us2.user_name<<" "<<test_us2.dir_id<<endl;
    //    db.deleteUser(test_us);
    //user.user_name = string("dayim");
    //db.updateUser(user);
    //struct User test = db.getUser(user.user_id);
    //cout<<test.user_id<<" "<<test.user_name<<endl;
    /*nar::Machine machine;
    machine.machine_id = string("test makinasi");
    machine.user_id = 1;
    machine.machine_quota = 5000;
    machine.machine_diskSpace = 5000;
    nar::File file;
    file.file_id = 2;
    file.file_name = string("test file2");
    file.file_size = 300;
    file.file_type = string("file");
    //db.insertFile(file);
    nar::UserToFile utff;
    utff.user_id = user.user_id;
    utff.file_id = file.file_id;
    //vector<struct File> vec = db.getUserFiles(user.user_id);
    //cout<<vec[0].file_id<<" "<<vec[1].file_id<<endl;
    //db.insertUserToFile(utff);
    //db.insertMachine(machine);
    //db.deleteMachine(machine);
    //db.deleteUser(user);
*/
    return 0;
}
