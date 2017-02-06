#ifndef NAR_MESSMACHINEREGISTER_H
#define NAR_MESSMACHINEREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include "../../nlohJson/json.hpp"

namespace nar {
    namespace messagetypes {
        namespace MachineRegister {
            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("machine_register")) {}
                    void sendMessage();
                    void receiveMessage(nlohmann::json macreg_req_recv);

            };
            class Response : public ResponseHeader {
                private:
                    std::string machine_id;
                public:
                    Response(int statcode , std::string _machine_id) : ResponseHeader(statcode, std::string("machine_register")), machine_id(_machine_id) {}
                    std::string& getMachineId();
                    void sendMessage();
                    void receiveMessage(nlohmann::json macreg_resp_recv);
            };
        }
    }
}


#endif
