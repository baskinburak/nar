#ifndef NAR_DBSTRUCTS_H
#define NAR_DBSTRUCTS_H
#include <time.h>
#include <string>

namespace nar {
    struct Directory{
        Directory():dir_id(-1),dir_size(0){}
        long long int dir_id;
        std::string dir_name;
        long long int dir_size;
        long long int change_time;
    };

    struct DirectoryTo{
        DirectoryTo():ForD(false){}
        long long int dir_id;
        long long int item_id;
        bool ForD;                  // 1--> Dir  0-> File
    };
    struct User{
        User():user_id(-1){}
        long long int  user_id;
        std::string user_name;
        std::string aes_crypted;
        std::string rsa_pri_crypted;
        std::string rsa_pub;
        long long int dir_id;
        long long int change_time;
    };

    struct Machine{
        Machine():machine_id(std::string("-1")),machine_quota(0),machine_diskSpace(0){}
        std::string machine_id;
        long long int  machine_quota;
        long long int machine_diskSpace;
        long long int change_time;
        long long int user_id;
    };

    struct UserToFile{
        long long int user_id;
        long long int file_id;
    };

    struct File{
        File():file_id(-1),file_size(0){}
        long long int file_id;
        std::string file_name;
        long long int file_size;
        long long int change_time;
    };

    struct Chunk{
        Chunk():chunk_id(-1),chunk_size(0){}
        long long int chunk_id;
        long long int file_id;
        long long int chunk_size;
        long long int change_time;
    };

    struct ChunkToMachine {
        long long int chunk_id;
        std::string machine_id;
    };

}

#endif //NAR_DBSTRUCTS_H
