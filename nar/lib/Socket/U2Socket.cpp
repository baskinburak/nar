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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <map>
#include <algorithm>
#include <boost/lexical_cast.hpp>

using std::cout;
using std::endl;
using boost::asio::ip::udp;
using std::pair;

nar::USocket::PacketGenerator::PacketGenerator(nar::File& file, unsigned int start_seqnum, unsigned int stream_id): _file(file), _next_seqnum(start_seqnum), _last_notaccessed_file_location(0), _stream_id(stream_id) {
    this->_pack_data_size = nar::Packet::PACKET_LEN - nar::Packet::HEADER_LEN;
}

nar::Packet* nar::USocket::PacketGenerator::operator[](unsigned int sqnm) {
    if(this->_packets.find(sqnm) != this->_packets.end())
        return this->_packets[sqnm];

    for(; this->_next_seqnum <= sqnm; this->_next_seqnum++) {
        if(this->_last_notaccessed_file_location >= this->_file.size()) {
            throw nar::Exception::USocket::PacketGenerator::NoMorePacket("No more packets to generate", this->_file.size());
        }
        unsigned long start = this->_last_notaccessed_file_location;
        unsigned long len = this->_pack_data_size;
        char buf[len];
        //cout << start << " " << this->_file.size() << endl;
        int rsize = this->_file.read(buf, start, len);
        this->_last_notaccessed_file_location += rsize;


        nar::Packet* pck = new nar::Packet();
        pck->make_data(this->_next_seqnum, this->_stream_id, buf, rsize);

        this->_packets[this->_next_seqnum] = pck;
    }
    return this->_packets[sqnm];
}

void nar::USocket::PacketGenerator::remove(unsigned int sqnm) {
    this->_packets.erase(sqnm);
}

unsigned int nar::USocket::rand_seqnum() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<unsigned int> uni(0, std::numeric_limits<unsigned int>::max());
    return uni(rng);
}

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
    this->_timer_flag = false;
    this->_nat_flag = false;
    this->_recv_flag = false;
    this->_ack_flag = false;
    this->_ran_flag = false;
    this->_synack_flag = false;

    this->_start_seqnum = this->rand_seqnum();
    this->_next_seqnum = this->_start_seqnum + 1;

    std::thread rcv_thr(nar::USocket::receive_thread, this);
    rcv_thr.detach();
}

nar::USocket::USocket(boost::asio::io_service& io_serv, const char* server_ip, unsigned short server_port, unsigned int stream_id): nar::USocket(io_serv, std::string(server_ip), server_port, stream_id) {

}

nar::USocket::~USocket() {
    _socket.close();
}

void nar::USocket::timer_thread(unsigned long usec, bool* stop_timer) {
//    cout << "timer hello " << stop_timer << endl;
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
//    cout << "timer leave " << stop_timer << endl;
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


         //rcvpck->print(); // debug


        if (rcvpck->is_syn() && rcvpck->is_ack()) {
            cout << "synack" << endl;
            sock->_syned = true;
            sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
            sock->_synack_flag = true;
            sock->_event_cv.notify_all();
            delete rcvpck;
        } else if(rcvpck->is_syn()) {
            cout << "syn" << endl;
            if(sock->_syned) {
                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            } else {
                sock->_syned = true;
                sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                rplpck.print();
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
                sock->_event_cv.notify_all();
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
            try {
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
                sock->_nat_flag = true;
                sock->_event_cv.notify_all();
            } catch(boost::system::system_error& Exp) {

            }
            delete rcvpck;
        } else if(rcvpck->is_data()) {
            nar::Packet rplpck;
            rplpck.make_ack(sock->_stream_id, rcvpck->get_seqnum());
            //cout << rcvpck->get_seqnum() << " " << sock->_expected_seqnum << endl;
            std::string pckstr = rplpck.make_packet();
            sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint);
            if(rcvpck->get_seqnum() == sock->_expected_seqnum) {
                sock->_recv_buffer.append(rcvpck->get_payload());
                sock->_recv_flag = true;
                sock->_expected_seqnum++;
                std::map<unsigned int, nar::Packet*>::iterator it;
                for(; (it = received_packets.find(sock->_expected_seqnum)) != received_packets.end(); sock->_expected_seqnum++) {
                    nar::Packet* pck = it->second;
                    sock->_recv_buffer.append(pck->get_payload());
                    received_packets.erase(it);
                    delete pck;
                }
                delete rcvpck;
                sock->_event_cv.notify_all();
            } else {
                if(received_packets.find(rcvpck->get_seqnum()) == received_packets.end()) {
                    received_packets[rcvpck->get_seqnum()] = rcvpck;
                } else {
                    delete rcvpck;
                }
            }
        } else if(rcvpck->is_ran()) {
            cout << "ran received" << endl;
            sock->_ran_packs.push_back(std::make_pair(rcvpck, new udp::endpoint(remote_endpoint)));
            sock->_ran_flag = true;
            sock->_event_cv.notify_all();
        } else {
            delete rcvpck;
        }
    }
}

void nar::USocket::randezvous_server() {
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    std::map<unsigned int, udp::endpoint* > prev_rands;
    while(true) {
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
            this->_ran_flag = false;
        }
        this->_event_cv.wait(lck);
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
                    cout << "lexical cast: " << boost::lexical_cast<std::string>(this->_peer_endpoint) << endl;
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
        if(this->_nat_flag) {
            this->_nat_flag = false;
            stop_timer(stp_tmr);
            break;
        } else if(this->_timer_flag) {
            this->_timer_flag = false;
        }
        this->_event_cv.wait(lck);
    }

    cout << "randezvous successful." << endl; // debug


     /*
     need syn/ack
     */
    nar::Packet syn_packet;
    syn_packet.make_syn(this->_stream_id, this->_start_seqnum);
    std::string synpck = syn_packet.make_packet();

    while(true) {
        this->_socket.send_to(boost::asio::buffer(synpck), this->_peer_endpoint);
        bool *stp_tmr = this->start_timer(1000000);
        if(this->_synack_flag) {
            this->_synack_flag = false;
            stop_timer(stp_tmr);
            break;
        } else if(this->_timer_flag) {
            this->_timer_flag = false;
        }
        this->_event_cv.wait(lck);
    }


    cout << "syn successful, my next: " << this->_next_seqnum << ", remote next: " << this->_expected_seqnum << endl;

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

bool nar::USocket::send(nar::File& file, unsigned long start, unsigned long len) {
    nar::USocket::PacketGenerator pckgen(file, this->_next_seqnum, this->_stream_id);
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    double window_size = 64; // packets
    unsigned int used_window = 0; // packets
    double rtt = 1000000; // microseconds
    double devrtt = 0; // microseconds
    std::set<unsigned int> sent_seqnums;
    std::map<unsigned int, boost::posix_time::ptime> sent_times;
    unsigned long timer_seqnum;
    bool* stp_tmr;
    bool all_generated = false;
    while(true) {
        //cout << "send loop" << endl;
        if(!all_generated) {
            //cout << "not all generated " << window_size << " " << used_window << endl;
            for(; used_window < window_size; used_window++) {
                try {
                    nar::Packet* pck = pckgen[this->_next_seqnum];
                    std::string pckstr = pck->make_packet();
                    //cout << "send with sqnm: " << this->_next_seqnum << endl;
                    //exit(0);
                    this->_socket.send_to(boost::asio::buffer(pckstr), this->_peer_endpoint);
                    sent_times[this->_next_seqnum] = boost::posix_time::microsec_clock::universal_time();
                    sent_seqnums.insert(this->_next_seqnum);
                    this->_acks.erase(this->_next_seqnum);
                    if(used_window == 0) {
                        timer_seqnum = pck->get_seqnum();
                        stp_tmr = start_timer(rtt + 4*devrtt);
                    }
                    this->_next_seqnum++;
                } catch(nar::Exception::USocket::PacketGenerator::NoMorePacket& Exp) {
                    //cout << "no more packet" << endl;
                    all_generated = true;
                    break;
                }
            }
        }

        if(this->_ack_flag) {
            //cout << "ack" << endl;
            bool timer_stopped = false;
            for(auto sqnm : this->_acks) {
                if(sent_seqnums.find(sqnm) != sent_seqnums.end()) {
                    used_window--;
                    sent_seqnums.erase(sqnm);
                    sent_times.erase(sqnm);
                    boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::universal_time() - sent_times[sqnm];
                    double currtt = diff.total_microseconds();
                    devrtt = 0.875 * devrtt + 0.125 * std::abs(rtt - currtt);
                    rtt = 0.875 * rtt + 0.125 * currtt;
                    if(sqnm == timer_seqnum) {
                        stop_timer(stp_tmr);
                        timer_stopped = true;
                    }
                    window_size += (1.0 / window_size);
                    pckgen.remove(sqnm);
                }
            }

            if(all_generated && sent_seqnums.size() == 0) {
                break;
            }

            if(timer_stopped) {
                auto it = sent_seqnums.begin();
                if(it != sent_seqnums.end()) {
                    unsigned int sqnm = *it;
                    boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::universal_time() - sent_times[sqnm];
                    unsigned int timer_usec = diff.total_microseconds();
                    if(rtt + 4*devrtt < timer_usec) {
                        //cout << timer_usec << " <<<<<" << endl;
                        stp_tmr = start_timer(0);
                    } else {
                        stp_tmr = start_timer(rtt + 4*devrtt - timer_usec);
                    }
                    timer_seqnum = sqnm;
                }
                this->_timer_flag = false;
            }
            this->_acks.clear();
            this->_ack_flag = false;
        }
        if(this->_timer_flag) {
            //cout << "timer" << endl;
            nar::Packet* pck = pckgen[timer_seqnum];
            std::string pckstr = pck->make_packet();
            sent_times[timer_seqnum] = boost::posix_time::microsec_clock::universal_time();
            this->_socket.send_to(boost::asio::buffer(pckstr), this->_peer_endpoint);
            this->_timer_flag = false;
            stp_tmr = start_timer(rtt + 4*devrtt);
            //window_size /= 2;
            window_size = std::max(window_size, 1.1);
            //cout << window_size << endl;
        }
        this->_event_cv.wait(lck);
    }
    lck.unlock();
}
