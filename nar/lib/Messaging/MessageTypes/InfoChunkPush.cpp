#include "InfoChunkPush.h"

long long int nar::messagetypes::InfoChunkPush::Request::get_chunkId() {
    return chunkId;
}

bool nar::messagetypes::InfoChunkPush::Request::get_success() {
    return success;
}
