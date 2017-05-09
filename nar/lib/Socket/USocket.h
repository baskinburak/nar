#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <boost/asio.hpp>
#include <nar/lib/Socket/Packet.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <mutex>
#include <set>
#include <nar/narnode/File/File.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <atomic>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <cryptopp/filters.h>

using boost::asio::ip::udp;
using std::pair;

namespace nar {
    /*
     *
     * Socket class that implements RDT over UDP with NAT.
     *
     * @author: baskin
     * @privar: _iserv, boost::asio::io_service*, stores the io service for access to OS functionalities.
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
     * @privar: _timer_flag, bool, is set whenever timer expires.
     * @privar: _recv_flag, bool, is set whenever a new data arrives to _recv_buffer.
     * @privar: _ack_flag, bool, is set whenever a new ack arrives to _acks.
     * @privar: _ran_flag, bool, is set whenever a new ran arrives to _ran_packs
     * @privar: _synack_flag, bool, is set whenever a new synack is received
     * @privar: _acks, std::set<unsigned int>, holds the set of acked seqnums.
     * @privar: _ran_packs, std::vector<pair<nar::Packet*, udp::endpoint*> >, stores the randezvous flagged packets with the endpoint. These packets are handled by other threads.(randezvous_server function or connect function
     * @tested: no
     *
    */
    class USocket {
        private:
            /*
             * A generator class that generates packets from a file on demand.
             *
             * @author: baskin
             * @privar: _stream_id, unsigned int, holds the stream id of the socket
             * @privar: _pack_data_size, unsigned short, holds the size of the data portion of nar::Packet
             * @privar: _file, nar::File&, holds the file that the packets are generated from
             * @privar: _next_seqnum, unsigned int, holds the next seqnum that no packet is generated yet
             * @privar: _last_notaccessed_file_location, unsigned long, holds the last non-accessed offset of the file
             * @privar: _packets, std::map<unsigned int, nar::Packet*>, holds the generated packets themselves
             * @tested: no
             *
            */
            class PacketGenerator {
                private:
                    unsigned int _stream_id;
                    unsigned short _pack_data_size; // holds the data area size of packets
                    nar::File& _file; //file from which packets will be generated
                    unsigned int _next_seqnum;
                    unsigned long _last_notaccessed_file_location;
                    unsigned long _end_file_location;
                    std::map<unsigned int, nar::Packet*> _packets;
                    CryptoPP::Weak::MD5 hash;
                public:
                    PacketGenerator(nar::File& file, unsigned int start_seqnum, unsigned int stream_id, unsigned long start, unsigned long len);
                    nar::Packet* operator[](unsigned int sqnm);
                    void remove(unsigned int sqnm);
                    std::string getHash();
            };

            class RandezvousEntry {
                public:
                    udp::endpoint* _fep;
                    udp::endpoint* _sep;
                    RandezvousEntry();
                    RandezvousEntry(udp::endpoint* fep);
                    void add_second(udp::endpoint* sep);
                    boost::posix_time::ptime _time;
            };

            static void poke_sock(unsigned short port, bool* stop, boost::asio::io_service* ioserv);
            static void receive_thread(nar::USocket* sock);
            void timer_thread(unsigned long usec, bool* stop_timer);

            bool* start_timer(unsigned long usec);
            void stop_timer(bool* stp_tmr);

            boost::asio::io_service* _iserv;

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
            std::mutex _work_mutex;
            std::condition_variable _event_cv;

            std::string _recv_buffer;

            bool _timer_flag;
            bool _nat_flag;
            bool _recv_flag;
            bool _ack_flag;
            bool _ran_flag;
            bool _synack_flag;


            bool _ran_done;

            std::set<unsigned int> _acks;

            std::vector<pair<nar::Packet*, udp::endpoint*> > _ran_packs;

            bool _exp_sqnm_set;
            std::atomic<bool> _close_sck;


            unsigned int rand_seqnum();
        public:
            /*
                stream_id = 0 for server
            */
            USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id);
            USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id);
            ~USocket();

            void randezvous_server();
            void connect();
            int recv(char* buf, int len);
            bool send(nar::File& file, unsigned long start, unsigned long len, std::string& hash );
            unsigned short get_port() const;
            void close();
    };
}

#endif
