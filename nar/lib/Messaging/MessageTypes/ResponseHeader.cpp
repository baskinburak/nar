#include "ResponseHeader.h"

int nar::messagetypes::ResponseHeader::get_statuscode() {
    return statuscode;
}

std::string& nar::messagetypes::ResponseHeader::get_replyto() {
    return replyto;
}

nlohmann::json nar::messagetypes::ResponseHeader::sendHead() {
    nlohmann::json header;
    header["reply_to"] = replyto;
    header["status_code"] = statuscode;
    return header;
}

void nar::messagetypes::ResponseHeader::recvFill(nlohmann::json & received) {
	replyto = received["reply_to"];
    statuscode = received["status_code"];
    return;
}
