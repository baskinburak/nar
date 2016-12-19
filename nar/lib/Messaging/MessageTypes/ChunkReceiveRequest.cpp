#include "ChunkReceiveRequest.h"

long long int nar::messagetypes::ChunkReceiveRequest::Request::get_chunkId() {
    return chunkId;
}

std::string nar::messagetypes::ChunkReceiveRequest::Request::get_token() {
    return token;
}

unsigned long nar::messagetypes::ChunkReceiveRequest::Request::get_chunkSize() {
    return chunkSize;
}
