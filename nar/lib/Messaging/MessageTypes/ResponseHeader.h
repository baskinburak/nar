#ifndef NAR_MESSRESPONSEHEADER_H
#define NAR_MESSRESPONSEHEADER_H
#include <string>

#include <vector>

#include <nar/lib/nlohJson/json.hpp>
namespace nar {
    namespace MessageTypes {
        class ResponseHeader {
            private:
                int _status_code;
                std::string _reply_to;
            public:
                ResponseHeader(int status_code, std::string reply_to): _status_code(status_code), _reply_to(reply_to){

                }
                int get_status_code();
                std::string& get_replyto();

                nlohmann::json send_head();

				//void send(Socket *sck, nlohmann::jsonn &json );
				void recv_fill(nlohmann::json & received);

        };
    }
}

#endif
