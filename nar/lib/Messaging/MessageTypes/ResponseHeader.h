#ifndef NAR_MESSRESPONSEHEADER_H
#define NAR_MESSRESPONSEHEADER_H
#include <string>

#include <vector>

#include <nar/lib/nlohJson/json.hpp>
namespace nar {
    namespace Messagetypes {
        class ResponseHeader {
            private:
                int statuscode;
                std::string replyto;
            public:
                ResponseHeader(int statcode, std::string r): statuscode(statcode), replyto(r){

                }
                int get_statuscode();
                std::string& get_replyto();

                nlohmann::json sendHead();

				//void send(Socket *sck, nlohmann::jsonn &json );
				void recvFill(nlohmann::json & received);

        };
    }
}

#endif
