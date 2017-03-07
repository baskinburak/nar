#include "RequestHeader.h"

std::string& nar::MessageTypes::RequestHeader::get_action() {
    return action;
}

nlohmann::json  nar::MessageTypes::RequestHeader::sendHead(){
    nlohmann::json header;
    header["action"] = action;
    return header;
}

void nar::MessageTypes::RequestHeader::recvFill( nlohmann::json & received) {
	action = received["action"];
	return;

}
