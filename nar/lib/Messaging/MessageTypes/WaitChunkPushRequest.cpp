#include "WaitChunkPushRequest.h"

std::string& nar::messagetypes::WaitChunkPushRequest::get_token() {
    return token;
}

std::string& nar::messagetypes::WaitChunkPushRequest::get_chunkid() {
    return chunkid;
}

unsigned long long int nar::messagetypes::WaitChunkPushRequest::get_chunksize() {
    return chunksize;
}
