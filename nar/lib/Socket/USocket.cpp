#include "USocket.h"
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
#include <stdexcept>

#include <crypto++/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>


using std::cout;
using std::endl;
using boost::asio::ip::udp;
using std::pair;

nar::USocket::PacketGenerator::PacketGenerator(nar::File& file, unsigned int start_seqnum, unsigned int stream_id, unsigned long start, unsigned long len): _file(file), _next_seqnum(start_seqnum), _last_notaccessed_file_location(start), _stream_id(stream_id), _end_file_location(start+len) {
    this->_end_file_location = std::min(this->_end_file_location, file.size());
    this->_pack_data_size = nar::Packet::PACKET_LEN - nar::Packet::HEADER_LEN;
}

nar::Packet* nar::USocket::PacketGenerator::operator[](unsigned int sqnm) {
    if(this->_packets.find(sqnm) != this->_packets.end())
        return this->_packets[sqnm];

    for(; this->_next_seqnum <= sqnm; this->_next_seqnum++) {
        if(this->_last_notaccessed_file_location >= this->_end_file_location) {
            throw nar::Exception::USocket::PacketGenerator::NoMorePacket("No more packets to generate", this->_file.size());
        }

        unsigned long len = std::min((unsigned long) this->_pack_data_size, this->_end_file_location - this->_last_notaccessed_file_location);
        char buf[len];
        int rsize = this->_file.read(buf,  this->_last_notaccessed_file_location, len);
        this->hash.Update((const byte*) buf, rsize);
        this->_last_notaccessed_file_location += rsize;


        nar::Packet* pck = new nar::Packet();
        pck->make_data(this->_next_seqnum, this->_stream_id, buf, rsize);

        this->_packets[this->_next_seqnum] = pck;
    }
    return this->_packets[sqnm];
}

std::string nar::USocket::PacketGenerator::getHash() {
    byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];
    this->hash.Final(digest);

    CryptoPP::HexEncoder encoder;
	std::string output;

	encoder.Attach( new CryptoPP::StringSink( output ) );
	encoder.Put( digest, sizeof(digest) );
	encoder.MessageEnd();
    return output;
}

void nar::USocket::PacketGenerator::remove(unsigned int sqnm) {
    if(this->_packets.find(sqnm) != this->_packets.end()) {
        delete this->_packets[sqnm];
        this->_packets.erase(sqnm);
    }
}

nar::USocket::RandezvousEntry::RandezvousEntry(): _fep(NULL), _sep(NULL) {
    _time = boost::posix_time::microsec_clock::local_time();
}

nar::USocket::RandezvousEntry::RandezvousEntry(udp::endpoint* fep): _fep(fep), _sep(NULL) {
    _time = boost::posix_time::microsec_clock::local_time();
}

void nar::USocket::RandezvousEntry::add_second(udp::endpoint* sep) {
    _sep = sep;
    _time = boost::posix_time::microsec_clock::local_time();
}

unsigned int nar::USocket::rand_seqnum() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<unsigned int> uni(0, std::numeric_limits<unsigned int>::max());
    return uni(rng);
}

unsigned short nar::USocket::get_port() const {
    return this->_port;
}

nar::USocket::USocket(boost::asio::io_service& io_serv, std::string server_ip, unsigned short server_port, unsigned int stream_id): _socket(io_serv), _stream_id(stream_id), _iserv(&io_serv) {
    static unsigned short udp_next_port = 10000;


    this->_active_timer_count = 0;
    this->_ran_done = false;
    this->_close_sck = false;
    this->_exp_sqnm_set = false;
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

    for(this->_port = udp_next_port; ; this->_port++) {
        this->_socket.bind(udp::endpoint(udp::v4(), this->_port), ec);
        if(!ec) break;
        if(this->_port == udp_next_port-1) {
            throw nar::Exception::USocket::NoAvailablePort("No available UDP port to bind");
        }
    }

    udp_next_port = this->_port+1;

    /*if(this->_port == 65536) {
        throw nar::Exception::USocket::NoAvailablePort("No available UDP port to bind");
    }*/


    this->_syned = false;
    this->_timer_flag = false;
    this->_nat_flag = false;
    this->_recv_flag = false;
    this->_ack_flag = false;
    this->_ran_flag = false;
    this->_synack_flag = false;
    this->_inactive_flag = false;

    this->_inactive_allow = NAR_INACTIVE_ALLOW;

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
    this->_atc_mutex.lock();
    this->_active_timer_count++;
    this->_atc_mutex.unlock();
    boost::asio::deadline_timer timer(*(this->_iserv), boost::posix_time::microseconds(usec));
    try {
        timer.wait();
    } catch(boost::system::system_error& Exp) {
    }


    try {
	    this->_work_mutex.lock();
	    if(!(*stop_timer)) {
		    this->_timer_flag = true;
		    this->_event_cv.notify_all();
	    }
	    delete stop_timer;
	    this->_work_mutex.unlock();
    } catch(std::system_error& ia) {
    }

    this->_atc_mutex.lock();
    this->_active_timer_count--;
    this->_atc_mutex.unlock();
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

void nar::USocket::poke_sock(unsigned short port, bool* stop, boost::asio::io_service* ioserv) {
    boost::asio::ip::udp::endpoint ep(boost::asio::ip::address(boost::asio::ip::address_v4(2130706433UL)), port);


    boost::asio::deadline_timer timer(*ioserv, boost::posix_time::microseconds(1000000));
    boost::asio::ip::udp::socket pke(*ioserv);
    pke.open(udp::v4());

    std::string dummy_string("a");

    while(!*stop) {

        timer.expires_from_now(boost::posix_time::microseconds(1000000));
        timer.wait();
        pke.send_to(boost::asio::buffer(dummy_string), ep);
    }


    delete stop;
}

void nar::USocket::receive_thread(nar::USocket* sock) {

    bool *stop_poke = new bool(false);
    std::thread poke_sock(nar::USocket::poke_sock, sock->get_port(), stop_poke, sock->_iserv);
    poke_sock.detach();

    char *buff = new char[nar::Packet::PACKET_LEN];
    boost::system::error_code ec;
    udp::endpoint remote_endpoint;
    std::unique_lock<std::mutex> lck(sock->_work_mutex);

    std::map<unsigned int, nar::Packet*> received_packets; // map from seqnum to packets themselves.

    while(true) {
        lck.unlock();
        std::size_t len = sock->_socket.receive_from(boost::asio::buffer(buff, nar::Packet::PACKET_LEN), remote_endpoint, 0, ec);
        lck.lock();

        if(sock->_close_sck) {
            /**stop_poke = true;
            delete buff;
            for(auto& pckent : received_packets) {
                delete pckent.second;
            }
            break;*/
        }


        if(len == 1 && buff[0] == 'a') {
            if(sock->_inactive_allow == 0) {
                sock->_inactive_flag = true;
                sock->_event_cv.notify_all();
            } else {
                sock->_inactive_allow--;
            }
            continue;
        }

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

        sock->_inactive_allow = NAR_INACTIVE_ALLOW;
         //rcvpck->print(); // debug


        if (rcvpck->is_syn() && rcvpck->is_ack()) {
            sock->_syned = true;
            if(!sock->_exp_sqnm_set) {
                sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
                sock->_exp_sqnm_set = true;
            }
            sock->_synack_flag = true;
            sock->_event_cv.notify_all();
            delete rcvpck;
        } else if(rcvpck->is_syn()) {
            if(sock->_syned) {
                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint, 0, ec);
            } else {
                sock->_syned = true;

                if(!sock->_exp_sqnm_set) {
                    sock->_expected_seqnum = rcvpck->get_seqnum() + 1;
                    sock->_exp_sqnm_set = true;
                }

                nar::Packet rplpck;
                rplpck.make_synack(sock->_stream_id, sock->_start_seqnum, rcvpck->get_seqnum());
                //rplpck.print();
                std::string pckstr = rplpck.make_packet();
                sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint, 0, ec);
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
            nar::Packet fin_pck;
            fin_pck.make_fin(sock->_stream_id);
            std::string finstr = fin_pck.make_packet();
            for(int i=0; i<100; i++) {
                sock->_socket.send_to(boost::asio::buffer(finstr), sock->_peer_endpoint, 0, ec);
                boost::asio::deadline_timer timer(*(sock->_iserv), boost::posix_time::microseconds(10000));
                timer.wait();
            }

            *stop_poke = true;
            delete buff;
            for(auto& pckent : received_packets) {
                delete pckent.second;
            }
            break;
        } else if(rcvpck->is_nat()) {
            nar::Packet rplpck;
            rplpck.make_nat(sock->_stream_id);
            std::string pckstr = rplpck.make_packet();
            sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint, 0, ec);
            sock->_nat_flag = true;
            sock->_event_cv.notify_all();
            delete rcvpck;
        } else if(rcvpck->is_data()) {
            nar::Packet rplpck;
            rplpck.make_ack(sock->_stream_id, rcvpck->get_seqnum());
            std::string pckstr = rplpck.make_packet();
            sock->_socket.send_to(boost::asio::buffer(pckstr), sock->_peer_endpoint, 0, ec);
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
            if(sock->_ran_done) {
                delete rcvpck;
            } else {
                sock->_ran_packs.push_back(std::make_pair(rcvpck, new udp::endpoint(remote_endpoint)));
                sock->_ran_flag = true;
                sock->_event_cv.notify_all();
            }
        } else {
            delete rcvpck;
        }
    }
    lck.unlock();

    sock->_atc_mutex.lock();
    while(sock->_active_timer_count > 0) {
        sock->_atc_mutex.unlock();
        boost::asio::deadline_timer timer(*(sock->_iserv), boost::posix_time::microseconds(1000000));
        timer.wait();
        sock->_atc_mutex.lock();
    }
    sock->_atc_mutex.unlock();
   // delete sock;
}

void nar::USocket::randezvous_server() {
    boost::system::error_code ec;
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    std::map<unsigned int, nar::USocket::RandezvousEntry> prev_rands;
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

                if(prev_rands.find(stream_id) != prev_rands.end()) {
                    nar::USocket::RandezvousEntry& rep = prev_rands[stream_id];
                    if(rep._fep == NULL) {
                        prev_rands[stream_id] = RandezvousEntry(ep);
                    } else if(*ep == *(rep._fep)) {
                        if(rep._sep) {
                            nar::Packet packet;
                            packet.make_ran_response(stream_id, *(rep._sep));
                            std::string pckstr = packet.make_packet();
                            rep._time = boost::posix_time::microsec_clock::local_time();
                            this->_socket.send_to(boost::asio::buffer(pckstr), *ep, 0, ec);
                        }
                    } else if(rep._sep == NULL) {
                        nar::Packet first_packet;
                        nar::Packet second_packet;
                        first_packet.make_ran_response(stream_id, *(rep._fep));
                        second_packet.make_ran_response(stream_id, *ep);

                        std::string firstpck = first_packet.make_packet();
                        std::string secondpck = second_packet.make_packet();

                        this->_socket.send_to(boost::asio::buffer(firstpck), *ep, 0, ec);
                        this->_socket.send_to(boost::asio::buffer(secondpck), *(rep._fep), 0, ec);

                        rep.add_second(ep);
                    } else if(*ep == *(rep._sep)){
                        nar::Packet packet;
                        packet.make_ran_response(stream_id, *(rep._fep));
                        std::string pckstr = packet.make_packet();
                        rep._time = boost::posix_time::microsec_clock::local_time();
                        this->_socket.send_to(boost::asio::buffer(pckstr), *ep, 0, ec);
                    }
                } else {
                    prev_rands[stream_id] = RandezvousEntry(ep);
                }
            }
            this->_ran_packs.clear();
            this->_ran_flag = false;
        }
        std::vector<unsigned int> delete_ids;
        for(auto& entr : prev_rands) {
            nar::USocket::RandezvousEntry& rep = entr.second;
            boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - rep._time;
            if(diff.total_milliseconds() > 60000) {
                delete_ids.push_back(entr.first);
            }
        }
        for(int i=0; i<delete_ids.size(); i++) {
            if(prev_rands[delete_ids[i]]._fep != NULL) {
                delete prev_rands[delete_ids[i]]._fep;
            }
            if(prev_rands[delete_ids[i]]._sep != NULL) {
                delete prev_rands[delete_ids[i]]._sep;
            }
            prev_rands.erase(delete_ids[i]);
        }
        this->_event_cv.wait(lck);
    }
    lck.unlock();
}

void nar::USocket::connect() {
    boost::system::error_code ec;
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    nar::Packet ran_packet;
    ran_packet.make_ran_request(this->_stream_id);
    std::string ranpck = ran_packet.make_packet();
    while(true) {
        this->_socket.send_to(boost::asio::buffer(ranpck), this->_server_endpoint, 0, ec);
        if(ec) {
            continue;
        }
        bool *stp_tmr = this->start_timer(1000000);
        if(this->_ran_flag) {
            this->_ran_flag = false;
            if(this->_ran_packs.size() == 0) {
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
        this->_event_cv.wait(lck);
    }

    this->_ran_done = true;
    for(int i=0; i < this->_ran_packs.size(); i++) {
        pair<nar::Packet*, udp::endpoint*>& pr = this->_ran_packs[i];
        delete pr.first;
        delete pr.second;
    }
    this->_ran_packs.clear();


    nar::Packet nat_packet;
    nat_packet.make_nat(this->_stream_id);
    std::string natpck = nat_packet.make_packet();

    while(true) {
        this->_socket.send_to(boost::asio::buffer(natpck), this->_peer_endpoint, 0, ec);
        if(ec) {
            continue;
        }
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

    nar::Packet syn_packet;
    syn_packet.make_syn(this->_stream_id, this->_start_seqnum);
    std::string synpck = syn_packet.make_packet();

    while(true) {
        for(int i=0; i<100; i++)
            this->_socket.send_to(boost::asio::buffer(synpck), this->_peer_endpoint, 0, ec);
        if(ec) {
            continue;
        }
        bool *stp_tmr = this->start_timer(1000000);
        if(this->_synack_flag) {
            this->_synack_flag = false;
            stop_timer(stp_tmr);
            break;
        } else if(this->_timer_flag) {
            this->_timer_flag = false;
        } else if(this->_syned) {
            stop_timer(stp_tmr);
            break;
        }
        this->_event_cv.wait(lck);
    }

    lck.unlock();

    
}


int nar::USocket::recv(char* buf, int len) {
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    while(true) {
        if(this->_recv_flag) {
            break;
        } else if(this->_inactive_flag) {
            this->_inactive_flag = false;
            std::cout << "Inactive recv thrown" << std::endl;
            throw nar::Exception::USocket::InactivePeer("Peer is inactive.");
        }
        this->_event_cv.wait(lck);
    }
    int read_len;
    try {
        read_len = this->_recv_buffer.copy(buf, len, 0);
        this->_recv_buffer.erase(0, read_len);
    } catch(std::out_of_range& exp) {
        read_len = 0;
    }
    this->_recv_flag = (this->_recv_buffer.size() > 0);
    lck.unlock();
    return read_len;
}

bool nar::USocket::send(nar::File& file, unsigned long start, unsigned long len, std::string& hash) {
    boost::system::error_code ec;
    nar::USocket::PacketGenerator pckgen(file, this->_next_seqnum, this->_stream_id, start, len);
    std::unique_lock<std::mutex> lck(this->_work_mutex);
    double window_size = 382; // packets
    unsigned int used_window = 0; // packets
    double rtt = 5000000; // microseconds
    double devrtt = 0; // microseconds
    std::set<unsigned int> resend_seqnums;
    std::map<unsigned int, boost::posix_time::ptime> sent_times;
    unsigned int timer_seqnum;
    bool* stp_tmr;
    bool all_generated = false;
    while(true) {
        if(!all_generated) {
            for(; used_window < window_size; used_window++) {
                try {
                    nar::Packet* pck = pckgen[this->_next_seqnum];
                    std::string pckstr = pck->make_packet();
                    do {
                        this->_socket.send_to(boost::asio::buffer(pckstr), this->_peer_endpoint, 0, ec);
                    } while(ec);
                    sent_times[this->_next_seqnum] = boost::posix_time::microsec_clock::universal_time();
                    this->_acks.erase(this->_next_seqnum);
                    if(used_window == 0) {
                        timer_seqnum = this->_next_seqnum;
                        stp_tmr = start_timer(rtt + 4*devrtt);
                    }
                    this->_next_seqnum++;
                } catch(nar::Exception::USocket::PacketGenerator::NoMorePacket& Exp) {
                    all_generated = true;
                    break;
                }
            }
        }

        if(this->_inactive_flag) {
            this->_inactive_flag = false;
            throw nar::Exception::USocket::InactivePeer("Peer is inactive.");
        } else if(this->_ack_flag) {
            bool timer_stopped = false;
            for(auto sqnm : this->_acks) {
                if(sent_times.find(sqnm) != sent_times.end()) {
                    used_window--;
                    if(resend_seqnums.find(sqnm) == resend_seqnums.end()) {
                        boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::universal_time() - sent_times[sqnm];
                        double currtt = diff.total_microseconds();
                        devrtt = 0.75 * devrtt + 0.25 * std::abs(rtt - currtt);
                        rtt = 0.875 * rtt + 0.125 * currtt;
                    }
                    if(sqnm == timer_seqnum) {
                        stop_timer(stp_tmr);
                        timer_stopped = true;
                    }
                    window_size += (1.0 / window_size);
                    pckgen.remove(sqnm);
                    sent_times.erase(sqnm);
                    resend_seqnums.erase(sqnm);
                }
            }

            if(all_generated && sent_times.size() == 0) {
                break;
            }

            if(timer_stopped) {
                auto it = sent_times.begin();
                if(it != sent_times.end()) {
                    unsigned int sqnm = it->first;
                    boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::universal_time() - sent_times[sqnm];
                    unsigned long timer_usec = diff.total_microseconds();
                    if(rtt + 4*devrtt < timer_usec) {
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
            nar::Packet* pck = pckgen[timer_seqnum];
            std::string pckstr = pck->make_packet();
            //sent_times[timer_seqnum] = boost::posix_time::microsec_clock::universal_time();
            do {
                this->_socket.send_to(boost::asio::buffer(pckstr), this->_peer_endpoint, 0, ec);
            } while(ec);
            resend_seqnums.insert(timer_seqnum);
            this->_timer_flag = false;
            stp_tmr = start_timer(rtt + 4*devrtt);
            window_size -= 0.5;
            window_size = std::max(window_size, 1.1);
            //std::cout << window_size << std::endl;
        }
        this->_event_cv.wait(lck);
    }
    lck.unlock();
    hash = pckgen.getHash();
    nar::Packet fin_pck;
    fin_pck.make_fin(this->_stream_id);
    std::string finstr = fin_pck.make_packet();
    for(int i=0; i<100; i++) {
        this->_socket.send_to(boost::asio::buffer(finstr), this->_peer_endpoint, 0, ec);
        boost::asio::deadline_timer timer(*(this->_iserv), boost::posix_time::microseconds(10000));
        timer.wait();
    }
    return true;
}

void nar::USocket::close() {
    this->_close_sck = true;
}
