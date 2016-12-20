#include "WaitChunkPullRequest.h"
std::string& nar::messagetypes::WaitChunkPullRequest::Request::get_token(){
    return token;
}
long long int nar::messagetypes::WaitChunkPullRequest::Request::get_chunk_id(){
    return chunk_id;
}

long long int  nar::messagetypes::WaitChunkPullRequest::Request::get_chunk_size(){
    return chunk_size;
}
