#include "FilePullRequest.h"

std::string& nar::messagetypes::FilePullRequest::Request::get_filename()
{
    return filename;
}

std::string& nar::messagetypes::FilePullRequest::Request::get_dir()
{
    return dir;
}

void nar::messagetypes::FilePullRequest::Response::add_element(struct nar::messagetypes::FilePullRequest::Response::PeerListElement& ele)
{
    elements.push_back(ele);
}

void nar::messagetypes::FilePullRequest::Response::add_element(std::string pid, std::string cid, std::string token)
{
    struct PeerListElement ele = {
        pid, //peer id
        cid, // chunk id
        token
    };
    elements.push_back(ele);
}

std::vector<struct nar::messagetypes::FilePullRequest::Response::PeerListElement>& nar::messagetypes::FilePullRequest::Response::get_elements()
{
    return elements;
}

std::string& nar::messagetypes::FilePullRequest::Response::get_fileid() {
    return fileid;
}

unsigned long long int nar::messagetypes::FilePullRequest::Response::get_chunksize() {
    return chunksize;
}
