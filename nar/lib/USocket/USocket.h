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
#include <utility>
#include <functional>
#include <queue>

namespace nar {
  class USocket {
    private:
      class AckComparator {
        public:
          bool operator()(const nar::Packet& pkt1, const nar::Packet& pkt2) {
            return pkt1.get_acknum() > pkt2.get_acknum();
          }
      };

      class SeqComparator {
        public:
          bool operator()(const nar::Packet& pkt1, const nar::Packet& pkt2) {
            return pkt1.get_seqnum() > pkt2.get_seqnum();
          }
      };
      /* protected by timer_mtx */
      int numberof_100us;
      std::mutex timer_mtx;

      int udp_sockfd;
      std::mutex receive_buffer_mtx;
      std::string receive_buffer;
      unsigned int stream_id;
      std::atomic<bool> stop_thread;


      /* protected by flag_mtx */
      bool syn_flag;
      bool synack_flag;
      bool ack_flag;
      bool ran_flag;
      bool recv_flag;
      bool nat_flag;
      bool timeout_flag;
      std::mutex flag_mtx;

      struct sockaddr peer_addr;

      unsigned short bind_port;
      
      std::condition_variable event_cv;
      std::mutex event_cv_mtx;

      /* protected by randevous_mtx */
      std::vector<std::pair<nar::Packet, struct sockaddr> > randevous_list;
      std::mutex randevous_mtx;

      /* protected by acks_list_mutex */
      std::mutex acks_list_mutex;
      std::priority_queue<nar::Packet, std::vector<nar::Packet>, AckComparator> acks;

      void receive_thread();
      void timer_thread();


    public:

      USocket(unsigned int);
      USocket(const USocket& rhs);
      ~USocket();
      void make_randevous(std::string server_ip, unsigned short server_port);
      void randevous_server(); // makes this socket a randevous server, call this to make socket arrange randevouses
      int recv(char* buf, int len);
      int send(char * buf, int len);
      unsigned short get_port();
  };
}

#endif
