#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <boost/asio.hpp>
#include <nar/lib/Socket/Packet.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <recursive_mutex>

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
     * @privar: _start_seqnum, unsigned int, stores the starting value of the seqnum.
     * @privar: _next_seqnum, unsigned int, stores the next seqnum for the next send operation.
     * @privar: _expected_seqnum, unsigned int, stores the expected seqnum from the peer.
     * @privar: _syned, bool, stores whether the socket is syned with a peer.
     * @privar: _work_mutex, std::recursive_mutex, any thread that does work on the socket must acquire this mutex. also used for mutex of cv that alerts that an event is occured.
     * @privar: _event_cv, std::condition_variable, this cv is notified whenever a new flag is set.
     * @privar: _recv_buffer, std::string, receive buffer in which the received data is stored as it comes.
     * @privar: _nat_flag, bool, is set whenever a nat packet is received.
     * @privar: _recv_flag, bool, is set whenever a new data arrives to _recv_buffer.
     * @privar: _ack_flag, bool, is set whenever a new ack arrives to _acks.
     * @privar: _ran_flag, bool, is set whenever a new ran arrives to _ran_packs
     * @privar: _acks, std::set<unsigned int>, holds the set of acked seqnums.
     * @privar: _ran_packs, std::vector<pair<nar::Packet*, udp::endpoint*> >, stores the randezvous flagged packets with the endpoint. These packets are handled by other threads.(randezvous_server function or connect function
     * @tested: no
     *
    */
    class USocket {
        private:
            static void receive_thread(nar::USocket* sock);
            static void timer_thread();
            std::string _server_ip;
            std::string _server_port;
            udp::socket _socket;
            udp::endpoint _server_endpoint;
            udp::endpoint _peer_endpoint;
            unsigned int _port;
            unsigned int _stream_id;
            unsigned int _start_seqnum;
            unsigned int _next_seqnum;
            unsigned int _expected_seqnum;
            bool _syned;
            std::recursive_mutex _work_mutex;
            std::condition_variable _event_cv;

            std::string _recv_buffer;

            bool _nat_flag;
            bool _recv_flag;
            bool _ack_flag;
            bool _ran_flag;

            std::set<unsigned int> _acks;

            std::vector<pair<nar::Packet*, udp::endpoint*> > _ran_packs;
        public:
            /*
                stream_id = 0 for server
            */
            USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id);
            USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id);
            ~USocket();

            void connect();
    };
}

#endif
