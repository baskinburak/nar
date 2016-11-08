// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

void nar::Socket::error(const char* msg, int errNo) const {
  std::cout  << "Error in "<< msg  << ": "<< strerror(errNo) << std::endl;
}

nar::Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}

nar::Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool nar::Socket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );
  int tmp = errno;
  if ( ! is_valid() ){
    error("Creating Socket",tmp);
    return false;
  }


  // TIME_WAIT - argh      DUNNO!!!!!!
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
  {
    int tmp = errno;
    error("Option Setting for Socket",tmp);
    return false;
  }

  return true;

}



bool nar::Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }



  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );

  int tmp = errno;
  if ( bind_return == -1 )
    {
      error("Binding the Socket",tmp);
      return false;
    }

  return true;
}


bool nar::Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

  int tmp = errno;
  if ( listen_return == -1 )
    {
      error("Listening",tmp);
      return false;
    }

  return true;
}


bool nar::Socket::accept ( Socket& new_socket, const sockaddr_in * addr  ) const
{
  if(addr == NULL)
  addr = &m_addr;

  int addr_length = sizeof ( addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) addr, ( socklen_t * ) &addr_length );


  int tmp = errno;
  if ( new_socket.m_sock <= 0 ) {
    error("Accepting Connection",tmp);
    return false;
  }
  else
    return true;
}


bool nar::Socket::send ( char *& offset, int &length ) const
{
  int len = ::send ( m_sock, offset, length, MSG_NOSIGNAL );
  int tmp = errno;
  if ( length == -1 )
  {
      error("Sending Data",tmp);
      return false;
  }
  offset += len;
  length -= tmp;
  return true;
}


int nar::Socket::recv ( char *& offset, int &length ) const
{
  //char buf [ MAXRECV + 1 ];

  //s = "";

  memset ( offset, 0, length );

  int status = ::recv ( m_sock, offset, length, 0 );
  int tmp = errno;
  if ( status == -1 )
    {
      error("Receiving Data",tmp);
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      
      offset += status;
      length -= status;
      return status;
    }
}



bool nar::Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT || status == 0 ){
    error("Connecting to a Wrong or Unsupported Address", errno);
    return false;
  } 

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;

    error("Connection to Host", errno);
    return false;
}


void nar::Socket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
