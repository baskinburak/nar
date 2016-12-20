#include "ChunkSendRequest.h"

std::string& nar::messagetypes::ChunkSendRequest::Request::get_token(){
    return token;
}
long long int nar::messagetypes::ChunkSendRequest::Request::get_chunk_id(){
    return chunk_id;
}

long long int  nar::messagetypes::ChunkSendRequest::Request::get_chunk_size(){
    return chunk_size;
}
