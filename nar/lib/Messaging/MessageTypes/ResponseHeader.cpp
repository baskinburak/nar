#include "ResponseHeader.h"

int nar::messagetypes::ResponseHeader::get_statuscode() {
    return statuscode;
}

std::string& nar::messagetypes::ResponseHeader::get_replyto() {
    return replyto;
}
void nar::messagetypes::ResponseHeader::sendVecFill(std::vector<SendMessObject> & sendVec) {
    SendMessObject headObj;
    headObj.name = std::string("header");
    SendMessElement act;
    act.name = std::string("replyto");
    act.type = (SendMessTypes)2;
    act.var = &replyto;
    SendMessElement sta;
    sta.name = std::string("statuscode");
    sta.type = (SendMessTypes)0;
    sta.var = &statuscode;
    headObj.var.push_back(act);
    headObj.var.push_back(sta);
    sendVec.push_back(headObj);
}
