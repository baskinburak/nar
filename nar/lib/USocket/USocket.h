#ifndef NAR_USOCKET_H
#define NAR_USOCKET_H

namespace nar {
  class USocket {
    private:
      std::string dest_ip;
      int dest_port;
      int udp_sockfd;
    public:
      USocket();
      void introduce(std::string& serv_ip, short serv_port, int streamid);
  };
}

#endif
