// Definition of the Socket class

#ifndef NAR_SOCKET_H
#define NAR_SOCKET_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;
namespace nar {
class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& , const sockaddr_in *addr = NULL ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( char * offset, int length ) const;
  int recv ( char * offset, int length ) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

  void close() const;

  bool is_active() const {
        int error = 0;
        socklen_t len = sizeof(error);
        int retval = getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &error, &len);
        return error == 0;
    }

  std::string get_dest_ip();

 private:

  int m_sock;
  sockaddr_in m_addr;

  void error(const char* msg, int errNo) const ;
};
}

#endif
