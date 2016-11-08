// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
using namespace nar;

ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

}


const ClientSocket& nar::ClientSocket::send ( char *& offset, int &length ) const
{
  if ( ! Socket::send ( offset,length ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}


// const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
// {
//   if ( ! Socket::recv ( s ) )
//     {
//       throw SocketException ( "Could not read from socket." );
//     }

//   return *this;
// }
