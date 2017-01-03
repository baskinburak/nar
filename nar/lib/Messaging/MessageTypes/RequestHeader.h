#ifndef NAR_MESSREQUESTHEADER_H
#define NAR_MESSREQUESTHEADER_H
#include <string>
#include <vector>
#include "MessageRS.h"
namespace nar {
    namespace messagetypes{
        class RequestHeader {
            private:
                std::string action;
            public:
                RequestHeader(std::string a): action(a) {

                }
                void sendVecFill(std::vector<SendMessObject> & sendVec);
                std::string& get_action();
        };
    }
}

#endif
