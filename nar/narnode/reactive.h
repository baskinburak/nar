#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/narnode/global.h>
#include <time.h>
#include <boost/filesystem.hpp>
namespace nar {
    void keep_alive(nar::Socket* sck, nar::Global* globals);
    void chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id);
    void chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, long long int chunk_id);
    void reactive_dispatcher(Global *globals);
    bool delete_chunk(unsigned int stream_id, nar::Global* globals, unsigned short rand_port, long long int chunk_id);
}
