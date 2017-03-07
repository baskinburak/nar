#include "U2Socket.h"
#include <boost/system/error_code.hpp>
#include "../Exception/Exception.h"
#include <string>
#include "Packet.h"

using boost::asio::ip::udp;

nar::USocket::USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port): _socket(io_serv) {
    _server_ip = server_ip;
    _server_port = std::to_string(server_port);
    udp::resolver resolver(io_serv);
    udp::resolver::query query(udp::v4(), _server_ip, _server_port);
    boost::system::error_code ec;
    udp::resolver::iterator iter = resolver.resolve(query, ec);
    if(ec) {
        throw nar::Exception::USocket::ResolveError("Resolve error occured in USocket constructor", _server_ip, server_port);
    }
    _server_endpoint = *iter;
}

nar::USocket::USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port): nar::USocket(io_serv, std::string(server_ip), server_port) {
    
}

nar::USocket::~USocket() {
    _socket.close();
}

static void nar::USocket::receive_thread(nar::USocket& sock) {
    char* recv_data[nar::Packet::PACKET_LEN];
    unsigned int expected_seqnum = 0;
    while(true) {        
        size_t len = sock._socket.receive_from(recv_data, nar::Packet::PACKET_LEN);
        if(len < nar::Packet::HEADER_LEN) continue;
        nar::Packet recvd;
        recvd.set_header(recv_data);
        try {
            recvd.set_payload_check(recv_data, nar::Packet::HEADER_LEN, len);
        } catch(nar::Exception::Packet::PacketLenMatchError& e) {
            continue;
        }

        if(recv.is_ran()) {
            sock._randezvous_process_list_mtx.lock();
            sock._randezvous_process_list.push_back(recvd);
            sock._randezvous_process_list_mtx.unlock();
            sock._flag_mtx.lock();
            sock._ran_flag = true;
            sock._flag_mtx.unlock();
            std::unique_lock<std::mutex> lck(sock._event_cv_mtx);
            this->_event_cv.notify_all();
        }
    }
}
