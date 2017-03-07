#include <nar/narserver/ServerGlobal.h>




void nar::ServerGlobal::read_start() {
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::ServerGlobal::read_end() {
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::ServerGlobal::write_start() {
    write_mtx.lock();
}
void nar::ServerGlobal::write_end() {
    write_mtx.unlock();
}


nar::ServerGlobal::ServerGlobal() {
    write_start();
    _db_name = string("nar");
    _db_user = string("root");
    _db_pass = string("123");
    _db = new nar::Database();
    _db->setUser(_db_user);
    _db->setPass(_db_pass);
    _db->setDbname(_db_name);
    _db->connect();
    write_end();
}

nar::ServerGlobal::ServerGlobal(string db_user, string db_pass, string db_name) {
    write_start();
    _db_name = db_name;
    _db_user = db_user;
    _db_pass = db_pass;
    _db = new nar::Database();
    _db->setUser(_db_user);
    _db->setPass(_db_pass);
    _db->setDbname(_db_name);
    _db->connect();
    write_end();
}


string nar::ServerGlobal::get_db_user() {
    read_start();
    string db_user =  _db_user;
    read_end();
    return db_user;
}

string nar::ServerGlobal::get_db_pass() {
    read_start();
    string db_pass =  _db_pass;
    read_end();
    return db_pass;
}

string nar::ServerGlobal::get_db_name() {
    read_start();
    string db_name =  _db_name;
    read_end();
    return db_name;
}

void nar::ServerGlobal::set_db_user(string db_user) {
    write_start();
    _db_user = db_user;
    write_end();
    return;
}

void nar::ServerGlobal::set_db_pass(string db_pass) {
    write_start();
    _db_pass = db_pass;
    write_end();
    return;
}

void nar::ServerGlobal::set_db_name(string db_name) {
    write_start();
    _db_name = db_name;
    write_end();
    return;
}
nar::Database* nar::ServerGlobal::get_db() {
    read_start();
    nar::Database* db = _db;
    read_end();
    return db;
}
