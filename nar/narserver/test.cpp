
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
    user.user_name = std::string("fatih");
    user.aes_crypted = std::string("aaaaaaaaa");
    user.rsa_pri_crypted = std::string("bbbbbbbbbb");
    user.rsa_pub = std::string("ccccccc");
    db.insertUser(user)
    nar::Directory dir;
    nar::DirectoryTo dirTo;
    nar::Chunk ch;
    nar::Machine mh;
    nar::File file;
    nar::UserToFile utff;
    nar::ChunkToMachine ctm;
    file.file_name = string("fatih.txt");
    db.insertFile(file);
    file.file_name = string("burak.txt");
    db.insertFile(file);
    file.file_name = string("utku.txt");
    db.insertFile(file);
    file.file_name = string("dogu.txt");
    db.insertFile(file);
    dir.dir_name = string("narrrrrrrrrrr");
    dir.dir_size = 12345;
    db.insertDirectory(dir);
    dirTo.dir_id =9;
    dirTo.item_id = 3;
    dirTo.ForD = 0;
    db.insertDirectoryTo(dirTo);
    dirTo.item_id = 4;
    dirTo.ForD = 0;
    db.insertDirectoryTo(dirTo);
    dirTo.item_id = 5;
    dirTo.ForD = 0;
    db.insertDirectoryTo(dirTo);
    dirTo.item_id = 6;
    dirTo.ForD = 0;
    db.insertDirectoryTo(dirTo);
    dirTo.item_id = 10;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);
    /*dir.dir_name = string("fatih");
    dir.dir_size = 12345;
    db.insertDirectory(dir);
    dir.dir_name = string("dogu");
    dir.dir_size = 12345;
    db.insertDirectory(dir);
    dir.dir_name = string("baskin");
    dir.dir_size = 12345;
    db.insertDirectory(dir);
    dir.dir_name = string("utku");
    dir.dir_size = 12345;
    db.insertDirectory(dir);*/
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
    //dir.dir_id = 1;
    //db.deleteDirectory(dir);
    /*dirTo.dir_id =3;
    dirTo.item_id = 5;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);
    dirTo.dir_id =5;
    dirTo.item_id = 6;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);
    dirTo.dir_id =6;
    dirTo.item_id =7;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);
    dirTo.dir_id =7;
    dirTo.item_id = 8;
    dirTo.ForD = 1;
    db.insertDirectoryTo(dirTo);*/
    //SELECT Dir_id, Dir_name, Dir_size, UNIX_TIMESTAMP(Change_time) As Time From Directories WHERE Dir_name = utku AND Dir_id IN (  SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN ( SELECT Dir_id From Directories WHERE Dir_name = baskin AND Dir_id IN (  SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN ( SELECT Dir_id From Directories WHERE Dir_name = dogu AND Dir_id IN (  SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN ( SELECT Dir_id From Directories WHERE Dir_name = fatih AND Dir_id IN (  SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN ( SELECT Dir_id From Directories WHERE Dir_name = / AND Dir_id IN (  SELECT Item_id FROM DirectoryTo WHERE ForD = 1 AND Dir_id IN (select Dir_id from Users where user_name= test1 ))))))))));
    //dir = db.findDirectoryId(std::string("test1"),std::string("fatih/dogu/baskin/utku/"));
    //std::cout<<dir.dir_id<<std::endl;
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
