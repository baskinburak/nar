#include "DirInfo.h"

std::string& nar::messagetypes::DirInfo::Request::get_dir() {
    return dir;
}

std::vector<struct nar::messagetypes::DirInfo::Response::DirListElement>& nar::messagetypes::DirInfo::Response::get_elements() {
    return elements;
}

void nar::messagetypes::DirInfo::Response::add_element(struct nar::messagetypes::DirInfo::Response::DirListElement& ele) {
    elements.push_back(ele);
}

void nar::messagetypes::DirInfo::Response::add_element(std::string ct, std::string ei, std::string en, unsigned long long int s, bool t) {
    struct DirListElement ele = {
        ct,
        ei,
        en,
        s,
        t
    };
    elements.push_back(ele);
}
void nar::messagetypes::DirInfo::Request::sendMessage(){

    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement d;
    d.name = "dir_name";
    d.type = (SendMessTypes)2;
    d.var = &dir;
    pay.var.push_back(d);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}
void nar::messagetypes::DirInfo::Response::sendMessage(){
    struct DirListElement {
        std::string changetime;
        std::string entityid;
        std::string entityname;
        unsigned long long int entitysize;
        bool type; // true for DIR, false for FILE
    };
    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement peerL;
    peerL.name = "peerlist";
    peerL.type =(SendMessTypes)5;
    std::vector<SendMessObject> objArr;
    for(int i = 0;i<elements.size();i++){
        SendMessObject tempObj;
        SendMessElement ct;
        ct.name = "change_time";
        ct.type = (SendMessTypes)2;
        ct.var = &elements[i].changetime;
        SendMessElement eid;
        eid.name = "entityid";
        eid.type = (SendMessTypes)2;
        eid.var = &elements[i].entityid;
        SendMessElement ename;
        ename.name = "entityname";
        ename.type = (SendMessTypes)2;
        ename.var = &elements[i].entityname;
        SendMessElement esize;
        esize.name = "entitysize";
        esize.type = (SendMessTypes)1;
        esize.var = &elements[i].entitysize;
        SendMessElement t;
        t.name = "type";
        t.type = (SendMessTypes)0;
        t.var = &elements[i].type;
        tempObj.var.push_back(ct);
        tempObj.var.push_back(eid);
        tempObj.var.push_back(ename);
        tempObj.var.push_back(esize);
        tempObj.var.push_back(t);
        objArr.push_back(tempObj);
    }
    peerL.var = &objArr;
    pay.var.push_back(peerL);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);
}
