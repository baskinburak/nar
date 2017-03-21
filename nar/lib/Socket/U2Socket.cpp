#include "U2Socket.h"
#include <boost/system/error_code.hpp>
#include <nar/lib/Exception/Exception.h>
#include <string>
#include <iostream>
#include "Packet.h"
#include <thread>
#include <random>
#include <limits>
#include <nar/narnode/File/File.h>

using std::cout;
using std::endl;
using boost::asio::ip::udp;
using std::pair;

nar::USocket::USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id): _socket(io_serv), _stream_id(stream_id), _iserv(&io_serv) {
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

    this->_syned = false;

    std::thread rcv_thr(nar::USocket::receive_thread, this);
    rcv_thr.detach();
}

nar::USocket::USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id): nar::USocket(io_serv, std::string(server_ip), server_port, stream_id) {
    
}

nar::USocket::~USocket() {
    _socket.close();
}

void nar::USocket::timer_thread(unsigned long usec, bool* stop_timer) {
    boost::asio::deadline_timer timer(*(this->_iserv), boost::posix_time::microseconds(usec));
    try {
        timer.wait();
    } catch(boost::system::system_error& Exp) {
    }

    std::unique_lock<std::mutex> lck(this->_work_mutex);
    if(!(*stop_timer)) {
        this->_timer_flag = true;
        this->_event_cv.notify_all();
    }
    delete stop_timer;
    lck.unlock();
}

bool* nar::USocket::start_timer(unsigned long usec) {
    bool* stp_tmr = new bool;
    *stp_tmr = false;
    std::thread thr(&nar::USocket::timer_thread, this, usec, stp_tmr);
    thr.detach();
    return stp_tmr;
}

void nar::USocket::stop_timer(bool* stp_tmr) {
    if(this->_timer_flag) {
        this->_timer_flag = false;
    } else {
        *stp_tmr = true;
    }
}

void nar::USocket::receive_thread(nar::USocket* sock) {
    char *buff = new char[nar::Packet::PACKET_LEN];
    boost::system::error_code ec;
    udp::endpoint remote_endpoint;
    std::unique_lock<std::mutex> lck(sock->_work_mutex);

    std::map<unsigned int, nar::Packet*> received_packets; // map from seqnum to packets themselves.

    while(true) {
        lck.unlock();
        std::size_t len = sock->_socket.receive_from(boost::asio::buffer(buff, nar::Packet::PACKET_LEN), remote_endpoint, 0, ec);
        lck.lock();
        if(ec) {
            continue;
        }

        if(len < nar::Packet::HEADER_LEN || len > nar::Packet::PACKET_LEN) {
            continue;
        }
        
        nar::Packet* rcvpck = new nar::Packet;
        rcvpck->set_header(buff);

  

        if(rcvpck->get_streamid() != sock->_stream_id) {
            continue;
        }


        try {
            rcvpck->set_payload_check(buff, nar::Packet::HEADER_LEN, len - nar::Packet::HEADER_LEN);
        } catch (nar::Exception::Packet::PacketLenMatchError& exp) {
            continue;
        }


        rcvpck->print(); // debug


        if (rcvpck->is_syn() && rcvpck->is_ack()) {
            sock->_syned = true;
            sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
            delete rcvpck;
        } else if(rcvpck->is_syn()) {
            if(sock->_syned) {
                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            } else {
                sock->_syned = true;
                sock->_event_cv.notify_all();
                sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
                std::random_device rd;
                std::mt19937 rng(rd());
                std::uniform_int_distribution<unsigned int> uni(0, std::numeric_limits<unsigned int>::max());
                sock->_start_seqnum = uni(rng);
                sock->_next_seqnum = sock->_start_seqnum + 1;
                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            }
            delete rcvpck;
        } else if(rcvpck->is_ack()) {
            unsigned int acknum = rcvpck->get_acknum();
            sock->_acks.insert(acknum);
            sock->_ack_flag = true;
            sock->_event_cv.notify_all();
            delete rcvpck;
        } else if(rcvpck->is_fin()) {
        } else if(rcvpck->is_nat()) {
            nar::Packet rplpck;
            rplpck.make_nat(sock->_stream_id);
            std::string pckstr = rplpck.make_packet();
            sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            sock->_nat_flag = true;
            sock->_event_cv.notify_all();
            delete rcvpck;
        } else if(rcvpck->is_data()) {
            nar::Packet rplpck;
            rplpck.make_ack(sock->_stream_id, rcvpck->get_seqnum());
            std::string pckstr = rplpck.make_packet();
            sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            if(rcvpck->get_seqnum() == sock->_expected_seqnum) {
                sock->_recv_buffer.append(rcvpck->get_payload());
                sock->_recv_flag = true;
                sock->_event_cv.notify_all();
                sock->_expected_seqnum++;
                std::map<unsigned int, nar::Packet*>::iterator it; 
                for(; (it = received_packets.find(sock->_expected_seqnum)) != received_packets.end(); sock->_expected_seqnum++) {
                    nar::Packet* pck = it->second;
                    sock->_recv_buffer.append(pck->get_payload());
                    received_packets.erase(it);
                    delete pck;
                }
                delete rcvpck;
            } else {
                if(received_packets.find(rcvpck->get_seqnum()) == received_packets.end()) {
                    received_packets[rcvpck->get_seqnum()] = rcvpck;
                } else {
                    delete rcvpck;
                }
            }
        } else if(rcvpck->is_ran()) {
            sock->_ran_packs.push_back(std::make_pair(rcvpck, new udp::endpoint(remote_endpoint)));
            sock->_ran_flag = true;
            sock->_event_cv.notify_all();
        } else {
            delete rcvpck;
        }
        
    }
    cout << sock->_server_ip << endl;
}

void nar::USocket::randezvous_server() {
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    std::map<unsigned int, udp::endpoint* > prev_rands; 
    while(true) {
        this->_event_cv.wait(lck);
        if(this->_ran_flag) {
            for(int i=0; i<this->_ran_packs.size(); i++) {
                pair<nar::Packet*, udp::endpoint*>& ran = this->_ran_packs[i];
                nar::Packet* ranpck = ran.first;
                udp::endpoint* ep = ran.second;

                unsigned int stream_id;

                try {
                    stream_id = ranpck->get_ran_streamid();
                } catch(nar::Exception::Packet::NoStreamId& Exp) {
                    continue;
                }

                if(prev_rands.find(stream_id) != prev_rands.end() && *ep != *prev_rands[stream_id]) {
                    nar::Packet first_packet;
                    nar::Packet second_packet;

                    udp::endpoint* prevep = prev_rands[stream_id];

                    first_packet.make_ran_response(stream_id, *ep);
                    second_packet.make_ran_response(stream_id, *prevep);

                    std::string firstpck = first_packet.make_packet();
                    std::string secondpck = second_packet.make_packet();

                    this->_socket.send_to(boost::asio::buffer(firstpck), *prevep);
                    this->_socket.send_to(boost::asio::buffer(secondpck), *ep);

                    prev_rands.erase(stream_id);
                } else {
                    prev_rands[stream_id] = ep;
                }
            }
            this->_ran_packs.clear();
        }
    }
    lck.unlock();
}

void nar::USocket::connect() {
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    nar::Packet ran_packet;
    ran_packet.make_ran_request(this->_stream_id);
    std::string ranpck = ran_packet.make_packet();
    while(true) {
        this->_socket.send_to(boost::asio::buffer(ranpck), this->_server_endpoint);
        bool *stp_tmr = this->start_timer(1000000);
        this->_event_cv.wait(lck);
        if(this->_ran_flag) {
            this->_ran_flag = false;
            // i have a randezvous response
            if(this->_ran_packs.size() == 0) {
                cout << "nar::USocket::connect THIS SHOULD NOT HAPPEN !" << endl;
                continue; // should not happen
            } else {
                try {
                    this->_peer_endpoint = this->_ran_packs[this->_ran_packs.size()-1].first->get_endpoint();
                } catch(nar::Exception::Packet::NotEndpoint& exp) {
                    continue;
                }
            }
            stop_timer(stp_tmr);
            break;
        } else if(this->_timer_flag) {
            this->_timer_flag = false;
        }
    }


    nar::Packet nat_packet;
    nat_packet.make_nat(this->_stream_id);
    std::string natpck = nat_packet.make_packet();

    while(true) {
        this->_socket.send_to(boost::asio::buffer(natpck), this->_peer_endpoint);
        bool *stp_tmr = this->start_timer(1000000);
        this->_event_cv.wait(lck);
        if(this->_nat_flag) {
            this->_nat_flag = false;
            stop_timer(stp_tmr);
            break;
        } else if(this->_timer_flag) {
            this->_timer_flag = false;
        }
    }

    cout << "randezvouz successful." << endl; // debug
    lck.unlock();
}


int nar::USocket::recv(char* buf, int len) {
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    while(!this->_recv_flag) {
        this->_event_cv.wait(lck);
    }
    int read_len = this->_recv_buffer.copy(buf, len, 0);
    this->_recv_buffer.erase(0, read_len);
    this->_recv_flag = this->_recv_buffer.size() > 0;
    lck.unlock();
    return read_len;
}

int nar::USocket::send(nar::File& file) {
    
}
