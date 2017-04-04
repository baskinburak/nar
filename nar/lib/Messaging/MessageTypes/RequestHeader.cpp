#include "RequestHeader.h"

std::string& nar::MessageTypes::RequestHeader::get_action() {
    return _action;
}

nlohmann::json  nar::MessageTypes::RequestHeader::send_head(){
    nlohmann::json header;
    header["action"] = _action;
    return header;
}

void nar::MessageTypes::RequestHeader::recv_fill( nlohmann::json & received) {
	_action = received["action"];
	return;

}
