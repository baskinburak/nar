#include "ResponseHeader.h"

int nar::MessageTypes::ResponseHeader::get_status_code() {
    return status_code;
}

std::string& nar::MessageTypes::ResponseHeader::get_replyto() {
    return replyto;
}

nlohmann::json nar::MessageTypes::ResponseHeader::sendHead() {
    nlohmann::json header;
    header["reply_to"] = replyto;
    header["status_code"] = status_code;
    return header;
}

void nar::MessageTypes::ResponseHeader::recvFill(nlohmann::json & received) {
	replyto = received["reply_to"];
    status_code = received["status_code"];
    return;
}
