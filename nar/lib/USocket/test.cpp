#include <iostream>
#include "Packet.h"
#include <string>
#include <iomanip>
#include <cassert>
#include "USocket.h"

int main() {
  std::cout << "Packet test; seqnum:0, syn:1, ack:0, fin:0, nat:0, data:0, ran:0, payload_len:13, stream_id: 12345, payload: keke naber ya";
  nar::Packet syn;
  syn.hdr_set_flags(1,0,0,0,0,0);
  syn.hdr_set_seqnum(0);
  syn.hdr_set_streamid(12345);
  syn.set_payload(std::string("keke naber ya"));
  std::string pck = syn.make_packet();
  std::string res;
  res.resize(24);
  res[0] = 0x00;
  res[1] = 0x00;
  res[2] = 0x00;
  res[3] = 0x00;
  res[4] = 0x80;
  res[5] = 0x00;
  res[6] = 0x0d;
  res[7]=0x0;
  res[8]=0x0;
  res[9]=0x30;
  res[10]=0x39;
  res[11]=0x6b;
  res[12]=0x65;
  res[13]=0x6b;
  res[14]=0x65;
  res[15]=0x20;
  res[16]=0x6e;
  res[17]=0x61;
  res[18]=0x62;
  res[19]=0x65;
  res[20]=0x72;
  res[21]=0x20;
  res[22]=0x79;
  res[23]=0x61;
  assert(pck.size() == res.size());
  for(int i=0; i<pck.size(); i++) {
    assert(pck[i] == res[i]);
  }
  std::cout << "....done" << std::endl;



  std::cout << "Packet test; seqnum:12, syn:0, ack:0, fin:0, nat:0, data:1, ran:0 payload_len:3, stream_id: 1212, payload: kop";
  nar::Packet data;
  data.hdr_set_seqnum(12);
  data.hdr_set_syn(0);
  data.hdr_set_ack(0);
  data.hdr_set_fin(0);
  data.hdr_set_nat(0);
  data.hdr_set_data(1);
  data.hdr_set_ran(0);
  data.hdr_set_payloadlen(3);
  data.hdr_set_streamid(1212);
  data.set_payload(std::string("kop"));
  pck = data.make_packet();
  res.clear();
  res.resize(14);
  res[0] = 0x00;
  res[1] = 0x00;
  res[2] = 0x00;
  res[3] = 0x0c;
  res[4] = 0x08;
  res[5] = 0x00;
  res[6] = 0x03;
  res[7]=0x0;
  res[8]=0x0;
  res[9]=0x04;
  res[10]=0xbc;
  res[11]=0x6b;
  res[12]=0x6f;
  res[13]=0x70;
  assert(pck.size() == res.size());
  for(int i=0; i<pck.size(); i++) {
    assert(res[i] == pck[i]);
  }
  std::cout << "....done" << std::endl;


  std::cout << "Packet test; seqnum:4294967295, syn:1, ack:1, fin:0, nat:0, data:0, ran:1, payload_len:0, stream_id: 65535, payload: ";
  nar::Packet synack(1,1,0,0,0,1,4294967295, 0, 65535,std::string(""));
  pck = synack.make_packet();
  res.clear();
  res.resize(11);
  res[0] = 0xff;
  res[1] = 0xff;
  res[2] = 0xff;
  res[3] = 0xff;
  res[4] = 0xc4;
  res[5] = 0x00;
  res[6] = 0x00;
  res[7]=0x00;
  res[8]=0x00;
  res[9]=0xff;
  res[10]=0xff;
  assert(pck.size() == res.size());
  for(int i=0; i<pck.size(); i++) {
    assert(res[i] == pck[i]);
  }
  std::cout << "....done" << std::endl;



  std::cout << "USocket test;" << std::endl;
  nar::USocket sock;
  std::cout << "tests done." << std::endl;
}
