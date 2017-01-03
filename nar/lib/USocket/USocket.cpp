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

nar::USocket::USocket() {
  if((udp_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) == -1)) {
    throw "USocket create error.";
  }
  int iMode = 0;
  ioctl(udp_sockfd, FIONBIO, &iMode);  
  std::cout << "sockfd: " << this->udp_sockfd << std::endl;

  /****** JUST FOR TEST ******/
  struct sockaddr_in me;
  me.sin_family = AF_INET;
  me.sin_port = htons(12345);
  me.sin_addr.s_addr = INADDR_ANY;
  bind(udp_sockfd, (struct sockaddr*) &me, sizeof(me));
  /****************************/


  stop_thread = false;
  stream_id = 0;

  std::thread rcv(&USocket::receive_thread, *this);
  rcv.detach();
  while(true) {
    receive_buffer_mutex.lock();
    std::cout << receive_buffer.size() << std::endl;
    receive_buffer_mutex.unlock();
  }
}

nar::USocket::USocket(const USocket& rhs) {
  
}

void nar::USocket::introduce(std::string& srv_ip, short srv_port, int streamid) {
  struct sockaddr_in si_remote;

  std::memset((char*) &si_remote, 0, sizeof(si_remote));

  si_remote.sin_family = AF_INET;
  si_remote.sin_port = htons(srv_port);

  if(inet_aton(srv_ip.c_str(), &si_remote.sin_addr) == 0) {
    throw "inet_aton error";
  }

  nar::Packet randevous;
  randevous.hdr_set_flags(0,0,0,0,0,1);
  randevous.hdr_set_seqnum(0);
  randevous.hdr_set_streamid(0);
  randevous.hdr_set_payloadlen(0);
  randevous.set_payload(std::string(""));

  std::string pck = randevous.make_packet();

  
}
