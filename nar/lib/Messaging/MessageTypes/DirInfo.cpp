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
