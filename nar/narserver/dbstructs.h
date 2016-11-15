#ifndef NAR_DBSTRUCTS_H
#define NAR_DBSTRUCTS_H

namespace nar {
    struct User{
        long long int user_id;
        std::string user_name;
        long long int quota;
        long long int disk_space;
        std::string cryptedKey;
    };

    struct Machine{
        std::string machine_id;
        long long int user_id;
        long long int machine_quota;
        long long int machine_diskspace;
    };

    struct UserToFiles{
        long long int user_id;
        long long int file_id;
    };

    struct File{
        long long int file_id;
        std::string file_name;
        long long int file_size;
        std::string file_type;
    };

    struct Chunk{
        long long int chunk_id;
        long long int file_id;
        long long int chunk_size;
    };

    struct ChunkToPeer {
        long long int chunk_id;
        std::string machine_id;
    };

}

#endif //NAR_DBSTRUCTS_H
