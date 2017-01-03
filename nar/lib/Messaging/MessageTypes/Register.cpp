#include "Register.h"
std::string& nar::messagetypes::Register::Request::get_username(){
    return username;
}
std::string& nar::messagetypes::Register::Request::get_aes(){
    return aes;
}
void nar::messagetypes::Register::Response::sendMessage() {
        std::vector<SendMessObject> sendVec;
        sendVecFill(sendVec);
        nar::messagetypes::sendMessage(sendVec);

}
void nar::messagetypes::Register::Request::sendMessage() {
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement ae;
    ae.name = "aes";
    ae.type = (SendMessTypes)2;
    ae.var = &aes;
    pay.var.push_back(ae);
    SendMessElement un;
    un.name = "username";
    un.type =(SendMessTypes)2;
    un.var = &username;
    pay.var.push_back(un);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}
