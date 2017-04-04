#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <time.h>
namespace nar {
    void machine_authenticate(nar::Socket* sck, nar::Global* globals);
    void keep_alive(nar::Socket* sck);
    void chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id);
    void chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, std::string chunk_id);
    void reactive_dispatcher(Global *globals);
}
