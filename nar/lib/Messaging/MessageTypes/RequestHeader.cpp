#include "RequestHeader.h"

std::string& nar::messagetypes::RequestHeader::get_action() {
    return action;
}
void  nar::messagetypes::RequestHeader::sendVecFill(std::vector<SendMessObject> & sendVec){
    SendMessObject headObj;
    headObj.name = std::string("header");
    SendMessElement act;
    act.name = std::string("action");
    act.type = (SendMessTypes)2;
    act.var = &action;
    headObj.var.push_back(act);
    sendVec.push_back(headObj);
}
