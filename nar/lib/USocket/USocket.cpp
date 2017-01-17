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

nar::USocket::USocket(unsigned int streamid) {
  this->stream_id = streamid;
  if((this->udp_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    throw "USocket create error.";
  }

  this->syn_flag = false;
  this->synack_flag = false;
  this->ack_flag = false;
  this->stop_thread = false;
  this->stream_id = streamid;

  std::thread rcv(&USocket::receive_thread, &*this);
  rcv.detach();
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
