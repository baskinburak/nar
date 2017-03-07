#include "U2Socket.h"
#include <boost/system/error_code.hpp>
#include "../Exception/Exception.h"
#include <string>
#include <iostream>
#include "Packet.h"
#include <thread>

using std::cout;
using std::endl;
using boost::asio::ip::udp;

nar::USocket::USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id): _socket(io_serv), _stream_id(stream_id) {
    this->_server_ip = server_ip;
    this->_server_port = std::to_string(server_port);
    udp::resolver resolver(io_serv);
    udp::resolver::query query(udp::v4(), _server_ip, _server_port);
    boost::system::error_code ec;
    udp::resolver::iterator iter = resolver.resolve(query, ec);
    if(ec) {
        throw nar::Exception::USocket::ResolveError("Resolve error occured in USocket constructor", _server_ip, server_port);
    }
    this->_server_endpoint = *iter;

    this->_socket.open(udp::v4());

    for(this->_port = 10000; this->_port < 65536; this->_port++) {
        this->_socket.bind(udp::endpoint(udp::v4(), this->_port), ec);
        if(!ec) break;
    }

    if(this->_port == 65536) {
        throw nar::Exception::USocket::NoAvailablePort("No available UDP port to bind");
    }

    cout << "port: " << this->_port << endl;

    std::thread rcv_thr(nar::USocket::receive_thread, this);
    rcv_thr.detach();
}

nar::USocket::USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id): nar::USocket(io_serv, std::string(server_ip), server_port, stream_id) {
    
}

nar::USocket::~USocket() {
    _socket.close();
}

void nar::USocket::receive_thread(nar::USocket* sock) {
    char *buff = new char[nar::Packet::PACKET_LEN];
    boost::system::error_code ec;
    udp::endpoint remote_endpoint;
    while(true) {
        std::size_t len = sock->_socket.receive_from(boost::asio::buffer(buff, nar::Packet::PACKET_LEN), remote_endpoint, 0, ec);
        if(ec) {
            continue;
        }

        if(len < nar::Packet::HEADER_LEN || len > nar::Packet::PACKET_LEN) {
            continue;
        }
        
        nar::Packet rcvpck;
        rcvpck.set_header(buff);
        try {
            rcvpck.set_payload_check(buff, nar::Packet::HEADER_LEN, len - nar::Packet::HEADER_LEN);
        } catch (nar::Exception::Packet::PacketLenMatchError& exp) {
            continue;
        }

        
    }
    cout << sock->_server_ip << endl;
}
