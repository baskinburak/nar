#include "PeerConnectionRequest.h"

std::string& nar::messagetypes::PeerConnectionRequest::Request::get_peer_id()
{
    return _peer_id;
}

std::vector<struct nar::messagetypes::PeerConnectionRequest::Response::PeerListElement>& nar::messagetypes::PeerConnectionRequest::Response::get_elements()
{
    return elements;
}

void nar::messagetypes::PeerConnectionRequest::Response::add_element(struct nar::messagetypes::PeerConnectionRequest::Response::PeerListElement& ele) {
    elements.push_back(ele);
}

void nar::messagetypes::PeerConnectionRequest::Response::add_element(std::string pIp, int pPort, std::string pId) {
    struct PeerListElement ele = {
        pIp,
        pPort,
        pIp
    };
    elements.push_back(ele);
}
