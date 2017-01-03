#ifndef NAR_PACKET_H
#define NAR_PACKET_H

#include <string>

namespace nar {
  class Packet {
    /*
      SEQNUM - 4 bytes
      FLAGS - 1 byte [SYN, ACK, FIN, NAT, DATA]
      PAYLOAD_LENGTH - 2 bytes
      STREAM_ID - 4 bytes
    */
    private:
      static std::string htons(unsigned short num);
      static std::string htonl(unsigned int num);
      static unsigned int ntoh(const std::string& in, int start, int len);
      char syn; // start stream with another peer
      char ack; // ack given seqnum
      char fin; // end the stream
      char nat; // mark packet for nat only
      char data; // mark packet as data packet
      unsigned int seqnum; // seqnum of the packet. packets constitute the sequence not bytes.
      unsigned short payload_len; // length of the payload
      unsigned int stream_id;
      std::string payload;
    public:
      static const unsigned short PACKET_LEN = 1000; // bytes
      static const unsigned short HEADER_LEN = 11; // bytes

      Packet(char s, char a, char f, char n, char d, unsigned int sqnm, unsigned short p, unsigned int strid, std::string pl): syn(s), ack(a), fin(f), nat(n), data(d), seqnum(sqnm), payload_len(p), stream_id(strid), payload(pl) {}
      Packet() {}

      void set_header(const std::string& hdr);
      void set_header(const char* hdr);
      void set_payload(const std::string& pl);
      void set_payload(const char* pl, int len);
      void set_payload(const std::string& pl, int start, int len);
      void set_payload(const char* pl);
      void hdr_set_syn(char n_syn);
      void hdr_set_ack(char n_ack);
      void hdr_set_fin(char n_fin);
      void hdr_set_nat(char n_nat);
      void hdr_set_data(char n_data);
      void hdr_set_flags(char n_syn, char n_ack, char n_fin, char n_nat, char n_data);
      void hdr_set_seqnum(unsigned int n_seqnum);
      void hdr_set_streamid(unsigned int n_strmid);
      void hdr_set_payloadlen(unsigned short n_pllen);

      bool is_syn() const;
      bool is_ack() const;
      bool is_fin() const;
      bool is_nat() const;
      bool is_data() const;
      std::string& get_payload();
      unsigned int get_streamid() const;
      unsigned int get_seqnum() const;
      unsigned short get_payloadlen() const;

      std::string make_packet() const;
  };
}

#endif
