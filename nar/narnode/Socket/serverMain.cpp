#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
int main (  )
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      while ( true )
		{
		  //std::string data;
		  char *buf;
		  buf = new char[50];
		  char *imlec = buf;
		  int len = 50;
		  new_sock.recv(imlec,len);
		  for(int i = 0; i < 25; ++i )
		  std::cout << buf[i] ;
		  std::cout << std::endl;
		  //new_sock << data;
		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}