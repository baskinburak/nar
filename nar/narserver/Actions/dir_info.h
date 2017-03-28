#ifndef NAR_SERVER_ACTIONS_DIR_INFO_H
#define NAR_SERVER_ACTIONS_DIR_INFO_H


#include <nar/lib/Messaging/MessageTypes/DirInfo.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/sockinfo.h>
#include <iostream>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::string;

namespace nar{
    namespace ServerActions {
        void get_user_dir_info(nar::SockInfo* inf, MessageTypes::DirInfo::Request & dir_req, nar::Database* db) {
            std::vector<struct nar::MessageTypes::DirInfo::Response::DirListElement > holder;
            int status_code  = 200;
            if(inf->isAuthenticated()) {
                string user_name = inf->getUser();
                string dir_name =  dir_req.get_dir();
                nar::User us= db->getUser(user_name);
                if(us.user_id != -1) {
                    std::vector<nar::File> files;
                    std::vector<nar::Directory> dirs;
                    if(dir_name.compare(string("")) == 0){
                        files = db->getDirectoryFile(us.dir_id);
                        dirs = db->getDirectoryDir(us.dir_id);
                    } else {
                        nar::Directory dir = db->findDirectoryId(user_name,dir_name);
                        if(dir.dir_id != -1){
                            files = db->getDirectoryFile(dir.dir_id);
                            dirs = db->getDirectoryDir(dir.dir_id);
                        }
                        else {
                            status_code = 302;
                        }
                    }
                    if(files.size() > 0){

                        for(int i = 0;i<files.size();i++) {

                            struct nar::MessageTypes::DirInfo::Response::DirListElement temp;
                            temp.entity_id= files[i].file_id;
                            temp.entity_name = files[i].file_name;
                            temp.entity_size = files[i].file_size;
                            temp.change_time = files[i].change_time;
                            temp.type = false;
                            holder.push_back(temp);
                        }
                    }
                    if(dirs.size() > 0){
                        for(int i = 0;i<dirs.size();i++) {
                            struct nar::MessageTypes::DirInfo::Response::DirListElement temp;
                            temp.entity_id = dirs[i].dir_id;
                            temp.entity_name = dirs[i].dir_name;
                            temp.entity_size = dirs[i].dir_size;
                            temp.change_time = dirs[i].change_time;
                            temp.type = false;
                            holder.push_back(temp);

                        }
                    }
                } else {
                    status_code = 301;
                }



            } else {
                status_code = 300;
            }

            nar::MessageTypes::DirInfo::Response dir_resp(status_code, holder);
            dir_resp.send_mess(inf->getSck());
            return ;
        }

    }
}



#endif
