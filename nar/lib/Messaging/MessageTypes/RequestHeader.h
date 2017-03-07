#ifndef NAR_MESSREQUESTHEADER_H
#define NAR_MESSREQUESTHEADER_H
#include <string>
#include <vector>
#include <nar/lib/nlohJson/json.hpp>
namespace nar {
    namespace MessageTypes{
        class RequestHeader {
            private:
                std::string action;
            public:
                RequestHeader(std::string a): action(a) {

                }
                std::string& get_action();


                nlohmann::json sendHead();
				void recvFill(nlohmann::json & received);
        };
    }
}

#endif
