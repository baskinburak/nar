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
void nar::messagetypes::WaitChunkPushRequest::Request::sendMessage() {

    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement tok;
    tok.name = "token";
    tok.type = (SendMessTypes)2;
    tok.var = &token;
    SendMessElement cid;
    cid.name = "chunk_id";
    cid.type = (SendMessTypes)1;
    cid.var = &chunk_id;
    SendMessElement csize;
    csize.name = "chunk_size";
    csize.type = (SendMessTypes)1;
    csize.var = &chunk_size;
    pay.var.push_back(tok);
    pay.var.push_back(cid);
    pay.var.push_back(csize);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}

void nar::messagetypes::WaitChunkPushRequest::Response::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    nar::messagetypes::sendMessage(sendVec);
}