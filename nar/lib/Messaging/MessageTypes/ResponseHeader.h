#ifndef NAR_MESSRESPONSEHEADER_H
#define NAR_MESSRESPONSEHEADER_H
#include <string>

#include <vector>

#include <nar/lib/nlohJson/json.hpp>
namespace nar {
    namespace MessageTypes {
        class ResponseHeader {
            private:
                int status_code;
                std::string replyto;
            public:
                ResponseHeader(int statcode, std::string r): status_code(statcode), replyto(r){

                }
                int get_status_code();
                std::string& get_replyto();

                nlohmann::json sendHead();

				//void send(Socket *sck, nlohmann::jsonn &json );
				void recvFill(nlohmann::json & received);

        };
    }
}

#endif
