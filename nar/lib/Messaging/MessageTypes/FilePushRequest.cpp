#include "FilePushRequest.h"

std::string& nar::messagetypes::FilePushRequest::Request::get_filename() {
    return filename;
}

unsigned long long int nar::messagetypes::FilePushRequest::Request::get_filesize() {
    return filesize;
}

std::string& nar::messagetypes::FilePushRequest::Request::get_dir() {
    return dir;
}

void nar::messagetypes::FilePushRequest::Response::add_element(struct nar::messagetypes::FilePushRequest::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::messagetypes::FilePushRequest::Response::add_element(std::string pid, std::string cid, std::string token) {
    struct PeerListElement ele = {
        pid, //peer id
        cid, // chunk id
        token
    };
    elements.push_back(ele);
}

std::vector<struct nar::messagetypes::FilePushRequest::Response::PeerListElement>& nar::messagetypes::FilePushRequest::Response::get_elements() {
    return elements;

}

std::string& nar::messagetypes::FilePushRequest::Response::get_fileid() {
    return fileid;
}

unsigned long long int nar::messagetypes::FilePushRequest::Response::get_chunksize() {
    return chunksize;
}

void nar::messagetypes::FilePushRequest::Request::sendMessage(){

    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement d;
    d.name = "dir";
    d.type = (SendMessTypes)2;
    d.var = &dir;
    SendMessElement fi;
    fi.name = "filename";
    fi.type = (SendMessTypes)2;
    fi.var = &filename;
    SendMessElement fsize;
    fsize.name = "filesize";
    fsize.type = (SendMessTypes)1;
    fsize.var = &filesize;
    pay.var.push_back(d);
    pay.var.push_back(fi);
    pay.var.push_back(fsize);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);

}
void nar::messagetypes::FilePushRequest::Response::sendMessage(){

    std::vector<SendMessObject> sendVec;
    sendVecFill(sendVec);
    SendMessObject pay;
    pay.name = "payload";
    SendMessElement csize;
    csize.name = "chunksize";
    csize.type = (SendMessTypes)1;
    csize.var = &chunksize;
    SendMessElement fi;
    fi.name = "fileid";
    fi.type = (SendMessTypes)2;
    fi.var = &fileid;
    SendMessElement peerL;
    peerL.name = "peerlist";
    peerL.type =(SendMessTypes)5;
    std::vector<SendMessObject> objArr;
    for(int i = 0;i<elements.size();i++){
        SendMessObject tempObj;
        SendMessElement pid;
        pid.name = "peerid";
        pid.type = (SendMessTypes)2;
        pid.var = &elements[i].peerid;
        SendMessElement cid;
        cid.name = "chunkid";
        cid.type = (SendMessTypes)2;
        cid.var = &elements[i].chunkid;
        SendMessElement tok;
        tok.name = "token";
        tok.type = (SendMessTypes)2;
        tok.var = &elements[i].token;
        tempObj.var.push_back(pid);
        tempObj.var.push_back(cid);
        tempObj.var.push_back(tok);
        objArr.push_back(tempObj);
    }
    peerL.var = &objArr;
    pay.var.push_back(csize);
    pay.var.push_back(fi);
    pay.var.push_back(peerL);
    sendVec.push_back(pay);
    nar::messagetypes::sendMessage(sendVec);

}
