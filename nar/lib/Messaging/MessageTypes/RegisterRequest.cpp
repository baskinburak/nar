#include "RegisterRequest.h"

std::string& nar::messagetypes::RegisterRequest::get_username() {
    return username;
}

std::string& nar::messagetypes::RegisterRequest::get_aes() {
    return aes;
}
