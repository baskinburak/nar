#ifndef NAR_MESSRESPONSEHEADER_H
#define NAR_MESSRESPONSEHEADER_H
#include <string>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/json.hpp>
#include <nar/lib/Messaging/MessageRS.h>
namespace nar {
    namespace messagetypes {
        class ResponseHeader {
            private:
                int statuscode;
                std::string replyto;
            public:
                ResponseHeader(int statcode, std::string r): statuscode(statcode), replyto(r){}
                int get_statuscode();
                std::string& get_replyto();
				//void send(Socket *sck, nlohmann::jsonn &json );
				void recvVFill(MessElement &header);
        };
    }
}

#endif
