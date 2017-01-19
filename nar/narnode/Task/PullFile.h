#ifndef NAR_PULLFILE_H
#define NAR_PULLFILE_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/lib/nlohJson/json.hpp>
#include <string>
#include <nar/lib/USocket/USocket.h>

namespace nar {
    namespace task {
        class PullFile : public nar::task::ITask {
            private:
                std::string file_name;
                unsigned long file_size;
                std::string cur_dir;

            public:
                void initialize();
                void constructRequestJson(nlohmann::json &j, std::string dirname);
                nar::Socket* connectToServer(nar::Global* globals);
                void sendRequestJson(nlohmann::json &j, nar::Socket *server_sck);
                void getResultJson(nlohmann::json &j_resp, nar::Socket *serverSck);
                void comeTogether(nlohmann::json &j_resp, nar::Socket *serverSck, std::string aes,nar::Global* globals);
                nar::Socket* connectToPeer(std::string peer_ip, int peer_port);
                void constructJsonforPeer(nlohmann::json &j, std::string dirname);
                void constructJsonforNewPeer(nlohmann::json::iterator &it, nlohmann::json &j, std::string peer_id);
                void getPeerInfo(std::string peerId, nar::Socket *serverSck,std::string _token);
                nar::Socket* sendTokenToPeer(nlohmann::json::iterator &it, nar::Socket *serverSck,unsigned long chunkSize);
                void pullFileFromPeer(nlohmann::json::iterator &it, nar::USocket& peerSck, unsigned long chunkSize, std::string aes, int file );
                                      
                void getResultJsonFake(nlohmann::json &j_resp, nar::Socket *serverSck);
                std::string getAes(nar::Socket *serverSck);

                PullFile(std::string fn, std::string curdir): file_name(fn), cur_dir(curdir) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}
#endif
