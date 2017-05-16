#ifndef NAR_DBEXTRASTRUCTS_H
#define NAR_DBEXTRASTRUCTS_H

namespace nar {
    namespace db{
        struct Session {
            sql::SQLString session_id;
            std::string machine_id;
            sql::SQLString join_time;
            sql::SQLString leave_time;
        };

        struct Directory{
            sql::SQLString dir_id;
            std::string dir_name;
            sql::SQLString dir_size;
            sql::SQLString change_time;
        };

        struct DirectoryTo{
            sql::SQLString dir_id;
            sql::SQLString item_id;
            sql::SQLString ForD;
        };

        struct User{
            sql::SQLString  user_id;
            std::string user_name;
            std::string aes_crypted;
            std::string rsa_pri_crypted;
            std::string rsa_pub;
            sql::SQLString dir_id;
            sql::SQLString change_time;
        };

        struct Machine{
            std::string machine_id;
            sql::SQLString  user_id;
            sql::SQLString  machine_quota;
            sql::SQLString machine_diskSpace;
            sql::SQLString change_time;
            sql::SQLString delete_list;
        };

        struct UserToFile{
            sql::SQLString user_id;
            sql::SQLString file_id;
            sql::SQLString change_time;
        };

        struct File{
            sql::SQLString file_id;
            std::string file_name;
            sql::SQLString file_size;
            sql::SQLString change_time;
        };

        struct Chunk{
            sql::SQLString chunk_id;
            sql::SQLString file_id;
            sql::SQLString chunk_size;
            sql::SQLString hashed;
        };

        struct ChunkToMachine {
            sql::SQLString chunk_id;
            std::string machine_id;
        };

        struct TimeTable {
            TimeTable() {
                time_keep.resize(24);
                time_count.resize(24);
            }
            std::string machine_id;
            std::vector<sql::SQLString> time_keep;
            std::vector<sql::SQLString> time_count;
            /*
            sql::SQLString i0_1;
            sql::SQLString i1_2;
            sql::SQLString i2_3;
            sql::SQLString i3_4;
            sql::SQLString i4_5;
            sql::SQLString i5_6;
            sql::SQLString i6_7;
            sql::SQLString i7_8;
            sql::SQLString i8_9;
            sql::SQLString i9_10;
            sql::SQLString i10_11;
            sql::SQLString i11_12;
            sql::SQLString i12_13;
            sql::SQLString i13_14;
            sql::SQLString i14_15;
            sql::SQLString i15_16;
            sql::SQLString i16_17;
            sql::SQLString i17_18;
            sql::SQLString i18_19;
            sql::SQLString i19_20;
            sql::SQLString i20_21;
            sql::SQLString i21_22;
            sql::SQLString i22_23;
            sql::SQLString i23_24;*/
            sql::SQLString last_update;
            sql::SQLString session_count;
        };
    }

}

#endif //NAR_DBEXTRASTRUCTS_H
