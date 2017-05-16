#ifndef NAR_DBSTRUCTS_H
#define NAR_DBSTRUCTS_H
#include <time.h>
#include <string>
#include <vector>

namespace nar {
    namespace DBStructs {
        struct Session {
            Session():session_id(-1){}
            long long int session_id;
            std::string machine_id;
            unsigned long join_time;
            unsigned long leave_time;
        };

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
            std::string delete_list;
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
            std::string hashed;
        };

        struct ChunkToMachine {
            long long int chunk_id;
            std::string machine_id;
        };
        struct TimeTable {
            TimeTable() {
                time_keep.resize(24);
                time_count.resize(24);
            }
            std::string machine_id;
            std::vector<unsigned long> time_keep;
            std::vector<unsigned long> time_count;
            /*unsigned long  i0_1;
            unsigned long i1_2;
            unsigned long i2_3;
            unsigned long i3_4;
            unsigned long i4_5;
            unsigned long i5_6;
            unsigned long i6_7;
            unsigned long i7_8;
            unsigned long i8_9;
            unsigned long i9_10;
            unsigned long i10_11;
            unsigned long i11_12;
            unsigned long i12_13;
            unsigned long i13_14;
            unsigned long i14_15;
            unsigned long i15_16;
            unsigned long i16_17;
            unsigned long i17_18;
            unsigned long i18_19;
            unsigned long i19_20;
            unsigned long i20_21;
            unsigned long i21_22;
            unsigned long i22_23;
            unsigned long i23_24;*/
            unsigned long last_update;
            unsigned long session_count;
        };
    }
}

#endif //NAR_DBSTRUCTS_H
