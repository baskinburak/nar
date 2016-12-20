#include "WaitChunkPushRequest.h"
std::string& nar::messagetypes::WaitChunkPushRequest::Request::get_token(){
    return token;
}
long long int nar::messagetypes::WaitChunkPushRequest::Request::get_chunk_id(){
    return chunk_id;
}

long long int  nar::messagetypes::WaitChunkPushRequest::Request::get_chunk_size(){
    return chunk_size;
}
