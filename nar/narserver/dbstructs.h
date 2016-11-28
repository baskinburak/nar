#ifndef NAR_DBSTRUCTS_H
#define NAR_DBSTRUCTS_H
#include <time.h>

namespace nar {
    struct Directory{
        long long int dir_id;
        std::string dir_name;
        long long int dir_size;
        time_t change_time;
    };

    struct DirectoryTo{
        long long int dir_id;
        long long int item_id;
        bool ForD;                  // 1--> Dir  0-> File
    };
    struct User{
        long long int  user_id;
        std::string user_name;
        long long int  quota;
        long long int disk_space;
        std::string cryptedKey;
        long long int dir_id;
        time_t change_time;
    };

    struct Machine{
        std::string machine_id;
        long long int user_id;
        long long int  machine_quota;
        long long int machine_diskSpace;
        time_t change_time;
    };

    struct UserToFile{
        long long int user_id;
        long long int file_id;
    };

    struct File{
        long long int file_id;
        std::string file_name;
        long long int file_size;
        std::string file_type;
        time_t change_time;
    };

    struct Chunk{
        long long int chunk_id;
        long long int file_id;
        long long int chunk_size;
        time_t change_time;
    };

    struct ChunkToMachine {
        long long int chunk_id;
        std::string machine_id;
    };

}

#endif //NAR_DBSTRUCTS_H
