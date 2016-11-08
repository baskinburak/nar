#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
using namespace nar;
int main (  )
{
  try
    {

      ClientSocket client_socket ( "127.0.0.1", 30000 );

      

      try
	{
    std::string str = "HALLO LENA ! DO YOU NOT?";
    char * writable = new char[24 + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[24] = '\0'; // don't forget the terminating 0
    int len = 24;//str.size();
    char *test = writable;
	  client_socket.send(writable, len);
    
    std::cout << *test << std::endl;
	  //client_socket >> reply;
	}
      catch ( SocketException& ) {std::cout << "NOOOOOOOOOOO" << std::endl;}

      std::cout << "We received this response from the server:\n\"" << "\"\n";;

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
