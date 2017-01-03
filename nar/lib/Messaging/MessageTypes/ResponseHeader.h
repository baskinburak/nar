#ifndef NAR_MESSRESPONSEHEADER_H
#define NAR_MESSRESPONSEHEADER_H
#include <string>
#include <vector>
#include "MessageRS.h"
namespace nar {
    namespace messagetypes {
        class ResponseHeader {
            private:
                int statuscode;
                std::string replyto;
            public:
                ResponseHeader(int statcode, std::string r): statuscode(statcode), replyto(r){
                    
                }
                int get_statuscode();
                std::string& get_replyto();
                void sendVecFill(std::vector<SendMessObject> & sendVec);
        };
    }
}

#endif
