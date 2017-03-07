#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <boost/asio.hpp>
#include <nar/lib/Socket/Packet.h>
#include <vector>
#include <mutex>
#include <condition_variable>

using boost::asio::ip::udp;

namespace nar {
    /*
     *
     * Socket class that implements RDT over UDP with NAT.
     *
     * @author: baskin
     * @privar: _server_ip, std::string, stores the randezvous server ip
     * @privar: _server_port, std::string, stores the randezvous server port
     * @privar: _socket, boost::asio::ip::udp::socket, udp socket handle to do operations over boost
     * @privar: _server_endpoint, boost::asio::ip::udp::endpoint, stores the randezvous server endpoint
     * @privar: _peer_endpoint, boost::asio::ip::udp::endpoint, stores the peer endpoint, needed for send
     * @privar: _port, unsigned int, stores the port number that _socket is bound
     * @privar: _stream_id, unsigned int, stores the stream id of the connection
     *
     * @tested: no
     *
    */
    class USocket {
        private:
            static void receive_thread(nar::USocket* sock);
            std::string _server_ip;
            std::string _server_port;
            udp::socket _socket;
            udp::endpoint _server_endpoint;
            udp::endpoint _peer_endpoint;
            unsigned int _port;
            unsigned int _stream_id;
        public:
            USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id);
            USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id);
            ~USocket();
    };
}

#endif
