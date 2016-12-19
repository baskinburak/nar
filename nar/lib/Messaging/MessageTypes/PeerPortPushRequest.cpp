#include "PeerPortPushRequest.h"

std::string& nar::messagetypes::PeerPortPushRequest::Request::get_token() {
    return token;
}

int nar::messagetypes::PeerPortPushRequest::Response::get_port() {
    return port;
}
