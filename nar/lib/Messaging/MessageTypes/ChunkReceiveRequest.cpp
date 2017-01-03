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

void nar::messagetypes::ChunkReceiveRequest::Request::recvMessage(Socket &sck){

	MessElement token;
	makeElement(token,"token",MSTRING);
	
	MessElement chunk_id;
	makeElement(token,"chunk_id",MLINT);

	MessElement chunk_size;
	makeElement(chunk_size,"chunk_size",MLINT);

	
	MessElement payload;
	MessObject  *pO = new MessObject();
	pO->var.push_back(token);
	pO->var.push_back(chunk_id);
	pO->var.push_back(chunk_size);
	payload.var = (void *) pO;

	MessElement header;
	recvVFill(header);
	vector<MessElement> arg;
	arg.push_back(header);
	arg.push_back(payload);
	recvFillMessage(arg);

		
	
	return;
}
