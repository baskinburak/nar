#ifndef NAR_MESSMACHINEREGISTER_H
#define NAR_MESSMACHINEREGISTER_H

#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/utility.h>
namespace nar {
    namespace MessageTypes {
        namespace MachineRegister {
            class Response : public ResponseHeader {
                private:
                    std::string machine_id;
                public:
                    Response(int statcode = -1 , std::string _machine_id  = std::string("")) : ResponseHeader(statcode, std::string("machine_register")), machine_id(_machine_id) {}
                    std::string& get_machine_id();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json macreg_resp_recv);
                    nlohmann::json test_json();
            };
            class Request : public RequestHeader {
                public:
                    Request(): RequestHeader(std::string("machine_register")) {}
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json macreg_req_recv);
                    nlohmann::json test_json();

            };

        }
    }
}


#endif
