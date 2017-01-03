#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H
#include <string>
#include <vector>
#include "Packet.h"
#include <iostream>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

namespace nar {
  class USocket {
    private:
      int udp_sockfd;
      std::mutex receive_buffer_mutex;
      std::vector<nar::Packet> receive_buffer;
      int stream_id;
      std::atomic<bool> stop_thread;

      void receive_thread() {
        char buf[nar::Packet::PACKET_LEN];
        struct sockaddr addr;
        unsigned int fromlen = sizeof(addr);
        while(!this->stop_thread) {
          std::cout << "PACKLEN: " << nar::Packet::PACKET_LEN << " " << this->udp_sockfd << std::endl;
          int len = recvfrom(this->udp_sockfd, buf, nar::Packet::PACKET_LEN, 0, &addr, &fromlen);
          int val = fcntl(this->udp_sockfd, F_GETFL, 0);
          bool is_blocking = !(val & O_NONBLOCK);
          std::cout << "huhu " << len << " " << is_blocking << std::endl;
          if(len < nar::Packet::HEADER_LEN) continue;
          nar::Packet recvd;
          recvd.set_header(buf);
          if(len != recvd.get_payloadlen() + nar::Packet::HEADER_LEN) continue;
          recvd.set_payload(buf + nar::Packet::HEADER_LEN);
          receive_buffer_mutex.lock();
          this->receive_buffer.push_back(recvd);
          receive_buffer_mutex.unlock();
        }
      }
    public:
      USocket();
      USocket(const USocket& rhs);
      void introduce(std::string& serv_ip, short serv_port, int streamid);
  };
}

#endif
