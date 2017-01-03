#include "ResponseHeader.h"

int nar::messagetypes::ResponseHeader::get_statuscode() {
    return statuscode;
}

std::string& nar::messagetypes::ResponseHeader::get_replyto() {
    return replyto;
}

void nar::messagetypes::ResponseHeader::recvVFill(nar::messagetypes::MessElement &header) {
	header.name = std::string("header");
	header.type = (MessTypes) 3;
	MessObject *headerO = new MessObject();
		MessElement status;
		status.name = std::string("status_code");
		status.type = (MessTypes) 0;
		MessElement reply;
		reply.name = std::string("reply_to");
		reply.type = (MessTypes) 2;
	headerO->var.push_back(status);
    headerO->var.push_back(reply);
	header.var = (void *) headerO;                                                            
	return;
}
