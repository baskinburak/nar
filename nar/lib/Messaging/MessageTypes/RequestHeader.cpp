#include "RequestHeader.h"

std::string& nar::messagetypes::RequestHeader::get_action() {
    return action;
}

void nar::messagetypes::RequestHeader::recvVFill( MessElement &header ) {
	header.name = "header";
	header.type = (MessTypes) 3;
	MessObject *headerO = new MessObject();
	MessElement action;
	action.name = "action";
	action.type = (MessTypes) 2;
	headerO->var.push_back(action);    
	header.var = (void *) headerO;                                                                    
	return; 
}
