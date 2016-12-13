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
