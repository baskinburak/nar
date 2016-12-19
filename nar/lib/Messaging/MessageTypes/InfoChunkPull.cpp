#include "InfoChunkPull.h"

long long int nar::messagetypes::InfoChunkPull::Request::get_chunkId() {
    return chunkId;
}

bool nar::messagetypes::InfoChunkPull::Request::get_success() {
    return success;
}
