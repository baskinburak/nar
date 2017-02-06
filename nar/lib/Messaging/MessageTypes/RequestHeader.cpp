#include "RequestHeader.h"

std::string& nar::messagetypes::RequestHeader::get_action() {
    return action;
}

nlohmann::json  nar::messagetypes::RequestHeader::sendHead(){
    nlohmann::json header;
    header["action"] = action;
    return header;
}

void nar::messagetypes::RequestHeader::recvFill( nlohmann::json & received) {
	action = received["action"];
	return;

}
