#include "RequestHeader.h"

std::string& nar::Messagetypes::RequestHeader::get_action() {
    return action;
}

nlohmann::json  nar::Messagetypes::RequestHeader::sendHead(){
    nlohmann::json header;
    header["action"] = action;
    return header;
}

void nar::Messagetypes::RequestHeader::recvFill( nlohmann::json & received) {
	action = received["action"];
	return;

}
