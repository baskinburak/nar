#include "Packet.h"
#include <cassert>

std::string nar::Packet::htons(unsigned short num) {
  std::string res;
  res.resize(2);
  res[1] = (num & 0xff);
  res[0] = (num & 0xff00) >> 8;
  return res;
}

std::string nar::Packet::htonl(unsigned int num) {
  std::string res;
  res.resize(4);
  res[3] = (num & 0xff);
  res[2] = (num & 0xff00) >> 8;
  res[1] = (num & 0xff0000) >> 16;
  res[0] = (num & 0xff000000) >> 24;
  return res;
}

unsigned int nar::Packet::ntoh(const std::string& inp, int start, int len) {
  unsigned int res = 0;
  for(int i=start; i<start+len; i++) {
    res = res << 8;
    res |= inp[i];
  }
  return res;
}

std::string nar::Packet::make_packet() const {
  assert(this->payload_len == (this->payload).size());
  char flags = (syn<<7) | (ack<<6) | (fin<<5) | (nat<<4) | (data<<3) | (ran<<2);
  std::string pckt_str;
  pckt_str = this->htonl(this->seqnum);
  pckt_str.push_back(flags);
  pckt_str += this->htons(this->payload_len);
  pckt_str += this->htonl(this->stream_id);
  if(this->payload_len > 0)
    pckt_str += this->payload;
  return pckt_str;
}

void nar::Packet::set_header(const std::string& hdr) {
  this->seqnum = this->ntoh(hdr, 0, 4);
  this->payload_len = this->ntoh(hdr, 5, 2);
  this->stream_id = this->ntoh(hdr, 7, 4);
  char flags = hdr[4];
  this->syn = (flags & (0x80)) >> 7;
  this->ack = (flags & (0x40)) >> 6;
  this->fin = (flags & (0x20)) >> 5;
  this->nat = (flags & (0x10)) >> 4;
  this->data = (flags & (0x8)) >> 3;
  this->ran = (flags & (0x4)) >> 2;
}

void nar::Packet::set_header(const char* hdr_c) {
  std::string hdr(hdr_c, HEADER_LEN);
  this->seqnum = this->ntoh(hdr, 0, 4);
  this->payload_len = this->ntoh(hdr, 5, 2);
  this->stream_id = this->ntoh(hdr, 7, 4);
  char flags = hdr[4];
  this->syn = (flags & (0x80)) >> 7;
  this->ack = (flags & (0x40)) >> 6;
  this->fin = (flags & (0x20)) >> 5;
  this->nat = (flags & (0x10)) >> 4;
  this->data = (flags & (0x8)) >> 3;
  this->ran = (flags & (0x4)) >> 2;
}

void nar::Packet::set_payload(const std::string& pl) {
  this->payload_len = pl.size();
  this->payload = pl;
}

void nar::Packet::set_payload(const std::string& pl, int start, int len) {
  this->payload_len = len;
  this->payload = std::string(pl, start, len);
}

void nar::Packet::set_payload(const char* pl, int len) {
  this->payload_len = len;
  this->payload = std::string(pl, len);
}

void nar::Packet::set_payload(const char* pl) {
  this->payload = std::string(pl, this->payload_len);
}

void nar::Packet::hdr_set_syn(char n_syn) {
  assert(n_syn == 0 || n_syn == 1);
  this->syn = n_syn;
}
void nar::Packet::hdr_set_ack(char n_ack) {
  assert(n_ack == 0 || n_ack == 1);
  this->ack = n_ack;
}
void nar::Packet::hdr_set_fin(char n_fin) {
  assert(n_fin == 0 || n_fin == 1);
  this->fin = n_fin;
}
void nar::Packet::hdr_set_nat(char n_nat) {
  assert(n_nat == 0 || n_nat == 1);
  this->nat = n_nat;
}
void nar::Packet::hdr_set_data(char n_data) {
  assert(n_data == 0 || n_data == 1);
  this->data = n_data;
}
void nar::Packet::hdr_set_ran(char n_ran) {
  assert(n_ran == 0 || n_ran == 1);
  this->ran = n_ran;
}
void nar::Packet::hdr_set_seqnum(unsigned int n_seqnum) {
  this->seqnum = n_seqnum;
}
void nar::Packet::hdr_set_streamid(unsigned int n_strmid) {
  this->stream_id = n_strmid;
}
void nar::Packet::hdr_set_payloadlen(unsigned short n_pllen) {
  this->payload_len = n_pllen;
}

void nar::Packet::hdr_set_flags(char n_syn, char n_ack, char n_fin, char n_nat, char n_data, char n_ran) {
  assert(n_syn == 0 || n_syn == 1);
  assert(n_ack == 0 || n_ack == 1);
  assert(n_fin == 0 || n_fin == 1);
  assert(n_nat == 0 || n_nat == 1);
  assert(n_data == 0 || n_data == 1);
  assert(n_ran == 0 || n_ran == 1);
  this->syn = n_syn;
  this->ack = n_ack;
  this->fin = n_fin;
  this->nat = n_nat;
  this->data = n_data;
  this->ran = n_ran;
}

bool nar::Packet::is_syn() const {
  return this->syn == 1;
}
bool nar::Packet::is_ack() const {
  return this->ack == 1;
}
bool nar::Packet::is_fin() const {
  return this->fin == 1;
}
bool nar::Packet::is_nat() const {
  return this->nat == 1;
}
bool nar::Packet::is_data() const {
  return this->data == 1;
}
bool nar::Packet::is_ran() const {
  return this->ran == 1;
}

std::string& nar::Packet::get_payload() {
  return this->payload;
}
unsigned int nar::Packet::get_streamid() const {
  return this->stream_id;
}
unsigned int nar::Packet::get_seqnum() const {
  return this->seqnum;
}
unsigned short nar::Packet::get_payloadlen() const {
  return this->payload_len;
}


