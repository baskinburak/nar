#ifndef NAR_MESSREGISTERREQUEST_H
#define NAR_MESSREGISTERREQUEST_H

#include "Request.h"
#include <string>

namespace nar {
    namespace messagetypes {
        class RegisterRequest : public Request {
            private:
                std::string username;
                std::string aes;
            public:
                RegisterRequest(std::string u, std::string a): Request(std::string("register")), username(u), aes(a) {}
                std::string& get_username();
                std::string& get_aes();
        };
    }
}

#endif
