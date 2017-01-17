#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <string>
#include <vector>
#include "Packet.h"
#include <iostream>
#include <atomic>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <condition_variable>
#include <mutex>

namespace nar {
  class USocket {
    private:
      class AckComparator {
        bool operator()(const nar::Packet& pkt1, const nar::Packet& pkt2) {
          return pkt1.get_acknum() < pkt2.get_acknum();
        }
      };

      class SeqComparator {
        bool operator()(const nar::Packet& pkt1, const nar::Packet& pkt2) {
          return pkt1.get_seqnum() < pkt2.get_seqnum();
        }
      };

      int udp_sockfd;
      std::mutex receive_buffer_mutex;
      std::string receive_buffer;
      unsigned int stream_id;
      std::atomic<bool> stop_thread;
      std::atomic<bool> syn_ok;

      bool syn_flag;
      bool synack_flag;
      bool ack_flag;
      std::mutex flag_mtx;
      
      std::condition_variable event_cv;
      std::mutex event_cv_mtx;

      std::priority_queue<nar::Packet, std::vector<nar::Packet>, AckComparator> acks;


      void receive_thread() {
        unsigned int expectedseqnum = 0;
        std::priority_queue<nar::Packet, std::vector<nar::Packet>, SeqComparator> received_packets;
        char buf[nar::Packet::PACKET_LEN];
        struct sockaddr addr;
        unsigned int fromlen = sizeof(addr);
        nar::Packet pqpacket;
        while(!this->stop_thread) {
          int len = recvfrom(this->udp_sockfd, buf, nar::Packet::PACKET_LEN, 0, &addr, &fromlen);
          if(len < nar::Packet::HEADER_LEN) continue;
          nar::Packet recvd;
          recvd.set_header(buf);
          recvd.set_payload(buf + nar::Packet::HEADER_LEN);

          if(len != recvd.get_payloadlen() + nar::Packet::HEADER_LEN) continue;
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
            sendto(this->udp_sockfd, packet.c_str(), packet.size(), 0, addr, fromlen);
            this->flag_mtx.lock();
            this->syn_flag = true;
            std::unique_lock<std::mutex> lck(this->event_cv_mtx);
            this->event_cv.notify_all();
            this->flag_mtx.unlock();
          } else if(recvd.is_ack()) {
            acks.push(recvd);
          } else if(recvd.is_nat()) {
          } else if(recvd.is_data()) {
          } else if(recvd.is_ran()) {
          }

          
          
        }
      }
    public:
      USocket();
      USocket(const USocket& rhs);
      void introduce(std::string& serv_ip, short serv_port, int streamid);
  };
}

#endif
