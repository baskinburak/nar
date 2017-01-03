#include "KeepAlive.h"

void nar::messagetypes::KeepAlive::Request::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    nar::messagetypes::sendMessage(sendVec);
}
void nar::messagetypes::KeepAlive::Response::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    nar::messagetypes::sendMessage(sendVec);
}
