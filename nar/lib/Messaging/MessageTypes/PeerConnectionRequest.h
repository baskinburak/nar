#ifndef NAR_MESSPEERCONNECTIONREQUEST_H
#define NAR_MESSPEERCONNECTIONREQUEST_H

#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <string>
#include <vector>

namespace nar {
    namespace messagetypes {
        namespace PeerConnectionRequest {
            class Request : public RequestHeader {
                private:
                    std::string _peer_id;
                public:
                    Request(std::string peer_id) : _peer_id(peer_id),
                            RequestHeader(std::string("peer_connection_request")) {}
                    std::string& get_peer_id();
            };

            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string peer_ip;
                        int peer_port;
                        std::string peer_id;
                    };
                    Response(int statcode, std::vector<struct PeerListElement>& e) :
                            ResponseHeader(statcode, std::string("peer_connection_request")), elements(e) {}
                    Response(int statcode) : ResponseHeader(statcode, std::string("peer_connection_request")) {}
                    std::vector<struct PeerListElement>& get_elements();
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string pIp, int pPort, std::string pId);
                private:
                    std::vector<struct PeerListElement> elements;
            };
        }
    }
}

#endif
