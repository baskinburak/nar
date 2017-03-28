#include "RequestHeader.h"

std::string& nar::MessageTypes::RequestHeader::get_action() {
    return action;
}

nlohmann::json  nar::MessageTypes::RequestHeader::send_head(){
    nlohmann::json header;
    header["action"] = action;
    return header;
}

void nar::MessageTypes::RequestHeader::recv_fill( nlohmann::json & received) {
	action = received["action"];
	return;

}
