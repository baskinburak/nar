#include "ResponseHeader.h"

int nar::Messagetypes::ResponseHeader::get_statuscode() {
    return statuscode;
}

std::string& nar::Messagetypes::ResponseHeader::get_replyto() {
    return replyto;
}

nlohmann::json nar::Messagetypes::ResponseHeader::sendHead() {
    nlohmann::json header;
    header["reply_to"] = replyto;
    header["status_code"] = statuscode;
    return header;
}

void nar::Messagetypes::ResponseHeader::recvFill(nlohmann::json & received) {
	replyto = received["reply_to"];
    statuscode = received["status_code"];
    return;
}
