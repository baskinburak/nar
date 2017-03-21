#include "ResponseHeader.h"

int nar::MessageTypes::ResponseHeader::get_status_code() {
    return _status_code;
}

std::string& nar::MessageTypes::ResponseHeader::get_replyto() {
    return _reply_to;
}

nlohmann::json nar::MessageTypes::ResponseHeader::send_head() {
    nlohmann::json header;
    header["reply_to"] = _reply_to;
    header["status_code"] = _status_code;
    return header;
}

void nar::MessageTypes::ResponseHeader::recv_fill(nlohmann::json & received) {
	_reply_to = received["reply_to"];
    _status_code = received["status_code"];
    return;
}
