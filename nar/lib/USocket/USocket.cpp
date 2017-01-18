#include "USocket.h"
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <cstring>
#include "Packet.h"
#include <thread>
#include <iostream>
#include <sys/ioctl.h>
#include <tuple>
#include <ctime>
#include <unistd.h>
#include <netdb.h>
#include <map>
#include <utility>
#include <algorithm>

nar::USocket::USocket(unsigned int streamid) {
  this->stream_id = streamid;
  if((this->udp_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    throw "USocket create error.";
  }

  this->bind_port = 45876;
  int res;

  do {
    this->bind_port++;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(this->bind_port);

    res = bind(this->udp_sockfd, (struct sockaddr*) &address, sizeof(struct sockaddr));
  } while(res == -1);

  this->syn_flag = false;
  this->synack_flag = false;
  this->ack_flag = false;
  this->ran_flag = false;
  this->recv_flag = false;
  this->nat_flag = false;
  this->timeout_flag = false;
  this->numberof_100us = -1;
  this->stop_thread = false;
  this->stream_id = streamid;

  std::thread rcv(&USocket::receive_thread, &*this);
  rcv.detach();

  std::thread tmr(&USocket::timer_thread, &*this);
  tmr.detach();
}

nar::USocket::USocket(const USocket& rhs) {
  
}

unsigned short nar::USocket::get_port() {
  return this->bind_port;
}

void nar::USocket::receive_thread() {
  unsigned int expectedseqnum = 0;
  std::priority_queue<nar::Packet, std::vector<nar::Packet>, SeqComparator> received_packets;
  char buf[nar::Packet::PACKET_LEN];
  struct sockaddr addr;
  unsigned int fromlen = sizeof(addr);
  nar::Packet pqpacket;
  while(!this->stop_thread) {
    int len = recvfrom(this->udp_sockfd, buf, nar::Packet::PACKET_LEN, 0, &addr, &fromlen);
    std::cout << "new data!" << std::endl;
    if(len < nar::Packet::HEADER_LEN) continue;
    nar::Packet recvd;
    recvd.set_header(buf);
    recvd.set_payload(buf + nar::Packet::HEADER_LEN);

    recvd.print();

    if(len != recvd.get_payloadlen() + nar::Packet::HEADER_LEN) continue;
    std::cout << "header len ok" << std::endl;

    if(this->stream_id != recvd.get_streamid()) continue;

    

    if(recvd.is_syn() && recvd.is_ack()) {
      this->flag_mtx.lock();
      this->synack_flag = true;
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      event_cv.notify_all();
      this->flag_mtx.unlock();
    } else if(recvd.is_syn()) {
      nar::Packet synack;
      synack.make_synack(this->stream_id);
      std::string packet = synack.make_packet();
      sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, &addr, fromlen);
      this->flag_mtx.lock();
      this->syn_flag = true;
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      this->event_cv.notify_all();
      this->flag_mtx.unlock();
    } else if(recvd.is_ack()) {
      this->acks_list_mutex.lock();
      this->acks.push(recvd);
      this->acks_list_mutex.unlock();
      this->flag_mtx.lock();
      this->ack_flag = true;
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      this->event_cv.notify_all();
      this->flag_mtx.unlock();
    } else if(recvd.is_nat()) {
      std::cout << "nat received" << std::endl;
      nar::Packet natback;
      natback.make_nat(this->stream_id);
      std::string packet = natback.make_packet();
      sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, &addr, fromlen);
      this->flag_mtx.lock();
      this->nat_flag = true;
      this->flag_mtx.unlock();
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      this->event_cv.notify_all();
    } else if(recvd.is_data()) {
      received_packets.push(recvd);
      nar::Packet ack;
      ack.make_ack(this->stream_id, recvd.get_seqnum());
      std::string packet = ack.make_packet();
      sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, &addr, fromlen);

      std::string res;
      bool new_exists = false;
      while(!received_packets.empty() && (pqpacket = received_packets.top()).get_seqnum() == expectedseqnum) {
        received_packets.pop();
        expectedseqnum++;
        res += pqpacket.get_payload();
        new_exists = true;
      }

      if(new_exists) {
        this->receive_buffer_mtx.lock();
        this->receive_buffer += res;
        this->receive_buffer_mtx.unlock();
        this->flag_mtx.lock();
        recv_flag = true;
        std::unique_lock<std::mutex> lck(this->event_cv_mtx);
        this->event_cv.notify_all();
        this->flag_mtx.unlock();
      }
    } else if(recvd.is_ran()) {
      std::cout << "it is ran" << std::endl;
      this->randevous_mtx.lock();
      this->randevous_list.push_back(std::make_pair(recvd, addr));
      this->randevous_mtx.unlock();
      this->flag_mtx.lock();
      this->ran_flag = true;
      this->flag_mtx.unlock();
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      this->event_cv.notify_all();
    }
  }
}

void nar::USocket::timer_thread() {
  bool expired = false;
  while(true) {
    //std::cout << this->numberof_100us << std::endl;
    usleep(100);
    this->timer_mtx.lock();
    this->numberof_100us--;
    if(this->numberof_100us == 0) {
      expired = true;
    } else {
      expired = false;
    }
    this->timer_mtx.unlock();
    if(expired) {
      std::cout << "expired" << std::endl;
      //exit(0);
      this->flag_mtx.lock();
      this->timeout_flag = true;
      std::unique_lock<std::mutex> lck(event_cv_mtx);
      event_cv.notify_all();
      this->flag_mtx.unlock();
    }
  }
}


void nar::USocket::randevous_server() {
  std::map<unsigned int, std::pair<nar::Packet, struct sockaddr> > randevous_map;
  std::map<unsigned int, std::tuple<struct sockaddr, struct sockaddr, time_t> > done_stream_ids;
  while(true) {
    std::unique_lock<std::mutex> lck(this->event_cv_mtx);
    this->event_cv.wait(lck);

    this->flag_mtx.lock();
    if(this->ran_flag) {
      this->randevous_mtx.lock();
      for(int i=0; i<this->randevous_list.size(); i++) {
        std::pair<nar::Packet, struct sockaddr>& rand = this->randevous_list[i];
        nar::Packet& pkt = rand.first;
        struct sockaddr& addr = rand.second;

        unsigned int streamid = pkt.get_streamid();
        if(done_stream_ids.find(streamid) != done_stream_ids.end()) {
          std::tuple<struct sockaddr, struct sockaddr, time_t> tpl = done_stream_ids[streamid];
          if(std::time(0) - std::get<2>(tpl) < 60) { // if more than 60 seconds passed, resend
            struct sockaddr f_sck = std::get<0>(tpl);
            struct sockaddr s_sck = std::get<1>(tpl);
            if(memcpy(&f_sck, &addr, sizeof(struct sockaddr)) == 0) {
              nar::Packet pkt;
              pkt.make_ran(streamid);
              pkt.set_payload((char*) &s_sck, sizeof(struct sockaddr));
              std::string pktstr = pkt.make_packet();
              sendto(this->udp_sockfd, pktstr.c_str(), pktstr.size(), 0, &addr, sizeof(struct sockaddr));
            } else if(memcpy(&s_sck, &addr, sizeof(struct sockaddr)) == 0) {
              nar::Packet pkt;
              pkt.make_ran(streamid);
              pkt.set_payload((char*) &f_sck, sizeof(struct sockaddr));
              std::string pktstr = pkt.make_packet();
              sendto(this->udp_sockfd, pktstr.c_str(), pktstr.size(), 0, &addr, sizeof(struct sockaddr));
            }
            continue;
          }
        }

        if(randevous_map.find(streamid) != randevous_map.end()) {
          std::pair<nar::Packet, struct sockaddr> sec_pair = randevous_map[streamid];
          if(memcmp(&sec_pair.second, &addr, sizeof(struct sockaddr)) != 0) {
            randevous_map.erase(streamid);
            nar::Packet pkt1;
            pkt1.make_ran(streamid);
            pkt1.set_payload((char*) &addr, sizeof(struct sockaddr));

            nar::Packet pkt2;
            pkt2.make_ran(streamid);
            pkt2.set_payload((char*) &sec_pair.second, sizeof(struct sockaddr));

            std::string pkt1str = pkt1.make_packet();
            std::string pkt2str = pkt2.make_packet();

            sendto(this->udp_sockfd, pkt1str.c_str(), pkt1str.size(), 0, &sec_pair.second, sizeof(struct sockaddr));
            sendto(this->udp_sockfd, pkt2str.c_str(), pkt2str.size(), 0, &addr, sizeof(struct sockaddr));
            done_stream_ids[streamid] = std::make_tuple(sec_pair.second, addr, std::time(0));
          }
        } else {
          randevous_map[streamid] = this->randevous_list[i];
        }
      }
      this->randevous_list.clear();
      this->randevous_mtx.unlock();
    }
    this->flag_mtx.unlock();
  }
}


void nar::USocket::make_randevous(std::string server_ip, short server_port) {
  struct sockaddr_in si_remote;

  std::memset((char*) &si_remote, 0, sizeof(si_remote));

  si_remote.sin_family = AF_INET;
  si_remote.sin_port = htons(server_port);

  if(inet_aton(server_ip.c_str(), &si_remote.sin_addr) == 0) {
    throw "inet_aton error";
  }

  nar::Packet ran_packet;
  std::cout << this->stream_id << " ---- stream id" << std::endl;
  ran_packet.make_ran(this->stream_id);
  std::string packet = ran_packet.make_packet();
  
  ran_packet.print();

  this->timer_mtx.lock();
  this->numberof_100us = 10000; // 1 secs
  this->timer_mtx.unlock();

  this->flag_mtx.lock();
  this->timeout_flag = false;
  this->flag_mtx.unlock();


  while(true) {
    sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, (struct sockaddr*)&si_remote, sizeof(struct sockaddr));
    std::unique_lock<std::mutex> lck(this->event_cv_mtx);
    this->event_cv.wait(lck);
    this->flag_mtx.lock();
    if(this->timeout_flag) {
      this->timer_mtx.lock();
      this->numberof_100us = 10000;
      this->timer_mtx.unlock();
      this->timeout_flag = false;
    }
    if(this->ran_flag) {
      this->randevous_mtx.lock();
      if(this->randevous_list.size() > 0) {
        nar::Packet& ran_pck = randevous_list[0].first;
        std::string payload = ran_pck.get_payload();
        memcpy(&this->peer_addr, payload.c_str(), payload.size());
        this->randevous_mtx.unlock();
        this->flag_mtx.unlock();
        this->timer_mtx.lock();
        this->numberof_100us = -1;
        this->timer_mtx.unlock();
        break;
      }
      this->randevous_mtx.unlock();
      this->ran_flag = false;
    }
    this->flag_mtx.unlock();
  }

  nar::Packet nat_packet;
  nat_packet.make_nat(this->stream_id);
  packet = nat_packet.make_packet();

  this->timer_mtx.lock();
  this->numberof_100us = 10000; // 1 secs
  this->timer_mtx.unlock();

  this->flag_mtx.lock();
  this->timeout_flag = false;
  this->flag_mtx.unlock();

  while(true) {
    sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, &this->peer_addr, sizeof(struct sockaddr));
    std::unique_lock<std::mutex> lck(this->event_cv_mtx);
    this->event_cv.wait(lck);
    this->flag_mtx.lock();
    if(this->timeout_flag) {
      this->timer_mtx.lock();
      this->numberof_100us = 10000;
      this->timer_mtx.unlock();
      this->timeout_flag = false;
    }
    if(nat_flag) {
      std::cout << "nat_flag" << std::endl;
      this->nat_flag = false;
      this->timer_mtx.lock();
      this->numberof_100us = -1;
      this->timer_mtx.unlock();
      this->flag_mtx.unlock();
      break;
    }
    this->flag_mtx.unlock();
  }
}


int nar::USocket::recv(char* buf, int len) {
  this->flag_mtx.lock();
  if(recv_flag) {
    this->receive_buffer_mtx.lock();
    int rlen = std::min(len, (int)this->receive_buffer.size());
    memcpy(buf, this->receive_buffer.c_str(), rlen);
    receive_buffer.erase(0, rlen);
    if(receive_buffer.size() == 0) {
      recv_flag = false;
    }
    this->receive_buffer_mtx.unlock();
    this->flag_mtx.unlock();
    return rlen;
  }

  
  while(!this->recv_flag) {
    this->flag_mtx.unlock();
    std::unique_lock<std::mutex> lck(this->event_cv_mtx);
    this->event_cv.wait(lck);
    this->flag_mtx.lock();
  }

  this->receive_buffer_mtx.lock();
  int rlen = std::min(len, (int)this->receive_buffer.size());
  memcpy(buf, this->receive_buffer.c_str(), rlen);
  receive_buffer.erase(0, rlen);
  if(receive_buffer.size() == 0) {
    recv_flag = false;
  }
  this->receive_buffer_mtx.unlock();
  this->flag_mtx.unlock();

  return rlen;
}

int nar::USocket::send(char* buf, int len) {
  std::vector<nar::Packet> packets;
  int MAX_PAYLOAD_LEN = nar::Packet::PACKET_LEN - nar::Packet::HEADER_LEN;

  nar::Packet pqpacket;

  int seqnum = 0;
  for(int cur=0; cur<len; cur+=MAX_PAYLOAD_LEN) {
    std::string payload(buf + cur, std::min(MAX_PAYLOAD_LEN, len-cur));
    nar::Packet data_packet;
    data_packet.make_data(seqnum++, this->stream_id, payload, payload.size());
    packets.push_back(data_packet);
  }

  for(int i=0; i<packets.size(); i++) {
    nar::Packet& cur_packet = packets[i];
    std::string packet = cur_packet.make_packet();
    
    this->timer_mtx.lock();
    this->numberof_100us = 10000; // 1 secs
    this->timer_mtx.unlock();

    this->flag_mtx.lock();
    this->timeout_flag = false;
    this->flag_mtx.unlock();

    while(true) {
      sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, &this->peer_addr, sizeof(struct sockaddr));
      std::unique_lock<std::mutex> lck(this->event_cv_mtx);
      this->event_cv.wait(lck);
      this->flag_mtx.lock();
      if(timeout_flag) {
        this->timer_mtx.lock();
        this->numberof_100us = 10000;
        this->timer_mtx.unlock();
        this->timeout_flag = false;
      }
      if(ack_flag) {
        this->acks_list_mutex.lock();
        while(!this->acks.empty()) {
          pqpacket = this->acks.top();
          this->acks.pop();
          if(pqpacket.get_acknum() == cur_packet.get_seqnum()) {
            this->acks_list_mutex.unlock();
            this->flag_mtx.unlock();
            break;
          }
        }
        this->acks_list_mutex.unlock();
      }
      this->flag_mtx.unlock();
    }
  }
}
