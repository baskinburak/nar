#include "Handshake.h"

std::string& nar::messagetypes::Handshake::Request::get_username() {
    return username;
}
void nar::messagetypes::Handshake::Request::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement un;
    un.name = "username";
    un.type = (SendMessTypes)2;
    un.var = &username;
    pay.var.push_back(un);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}
void nar::messagetypes::Handshake::Response::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    nar::messagetypes::sendMessage(sendVec);
}
