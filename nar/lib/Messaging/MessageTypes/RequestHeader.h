#ifndef NAR_MESSREQUESTHEADER_H
#define NAR_MESSREQUESTHEADER_H
#include <string>
#include <vector>
#include <nar/lib/nlohJson/json.hpp>
namespace nar {
    namespace MessageTypes{
        class RequestHeader {
            private:
                std::string _action;
            public:
                RequestHeader(std::string a): _action(a) {

                }
                std::string& get_action();


                nlohmann::json send_head();
				void recv_fill(nlohmann::json & received);
        };
    }
}

#endif
