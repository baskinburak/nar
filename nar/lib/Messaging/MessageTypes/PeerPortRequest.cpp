#include "PeerPortRequest.h"

int nar::messagetypes::PeerPortRequest::Response::get_port_number(){
    return port_number;
}

void nar::messagetypes::PeerPortRequest::Request::sendMessage(){
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    nar::messagetypes::sendMessage(sendVec);
}
void nar::messagetypes::PeerPortRequest::Response::sendMessage(){
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement pn;
    pn.name = "port_number";
    pn.type = (SendMessTypes)0;
    pn.var = &port_number;
    pay.var.push_back(pn);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}
