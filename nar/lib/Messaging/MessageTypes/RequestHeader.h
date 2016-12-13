#ifndef NAR_MESSREQUESTHEADER_H
#define NAR_MESSREQUESTHEADER_H
#include <string>
namespace nar {
    namespace messagetypes{
        class RequestHeader {
            private:
                std::string action;
            public:
                RequestHeader(std::string a): action(a) {}
                std::string& get_action();
        };
    }
}

#endif
