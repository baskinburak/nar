// Definition of the ClientSocket class

#ifndef NAR_CLIENTSOCKET_H
#define NAR_CLIENTSOCKET_H

#include "Socket.h"

namespace nar{
class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  const ClientSocket& send ( char * offset, int length ) const;
  const ClientSocket& recv ( char * offset, int length ) const;
};
}

#endif
