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
        User():user_id(-1),quota(0),disk_space(0){}
        long long int  user_id;
        std::string user_name;
        long long int  quota;
        long long int disk_space;
        std::string cryptedKey;
        long long int dir_id;
        long long int change_time;
    };

    struct Machine{
        Machine():machine_id(std::string("-1")),machine_quota(0),machine_diskSpace(0){}
        std::string machine_id;
        long long int user_id;
        long long int  machine_quota;
        long long int machine_diskSpace;
        long long int change_time;
    };

    struct UserToFile{
        long long int user_id;
        long long int file_id;
    };

    struct File{
        File():file_id(-1),file_size(0),file_type(std::string("file")){}
        long long int file_id;
        std::string file_name;
        long long int file_size;
        std::string file_type;
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
