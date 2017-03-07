#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <boost/asio.hpp>
#include <nar/lib/Socket/Packet.h>
#include <vector>
#include <mutex>
#include <condition_variable>

using boost::asio::ip::udp;

namespace nar {
    class USocket {
        private:
            friend static void receive_thread(nar::USocket& sock);
            std::string _server_ip;
            std::string _server_port;
            udp::socket _socket;
            udp::endpoint _server_endpoint;


            std::mutex _randezvous_process_list_mtx;
            std::vector<nar::Packet> _randezvous_process_list;

            std::mutex _flag_mtx;
            bool _ran_flag;

            std::condition_variable _event_cv;
            std::mutex _event_cv_mtx;
        public:
            USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port);
            USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port);
            ~USocket();
    };
}

#endif
