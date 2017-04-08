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
                    std::string _machine_id;
                public:
                    Response() : ResponseHeader(-1, std::string("machine_register")) {}
                    Response(int statcode , std::string machine_id ) : ResponseHeader(statcode, std::string("machine_register")), _machine_id(machine_id) {}
                    std::string& get_machine_id();
                    void send_mess(nar::Socket* skt);
                    void receive_message(nlohmann::json macreg_resp_recv);
                    nlohmann::json test_json();
            };
            class Request : public RequestHeader {
                private:
                    long long int _machine_quota;
                    long long int _machine_diskspace;
                public:
                    Request(): RequestHeader(std::string("machine_register")) {}
                    Request(long long int machine_quota, long long int machine_diskspace) : RequestHeader(std::string("machine_register")), _machine_quota(machine_quota), _machine_diskspace(machine_diskspace) {}
                    void send_mess(nar::Socket* skt, nar::MessageTypes::MachineRegister::Response& resp);
                    void receive_message(std::string& macreg_req_recv);
                    long long int get_machine_quota();
                    long long int get_machine_diskspace();
                    nlohmann::json test_json();

            };

        }
    }
}


#endif
