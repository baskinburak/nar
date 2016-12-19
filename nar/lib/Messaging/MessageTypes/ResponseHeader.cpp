#include "ResponseHeader.h"

int nar::messagetypes::ResponseHeader::get_statuscode() {
    return statuscode;
}

std::string& nar::messagetypes::ResponseHeader::get_replyto() {
    return replyto;
}
