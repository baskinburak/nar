// SocketException class


#ifndef NAR_SOCKETEXCEPTION_H
#define NAR_SOCKETEXCEPTION_H

#include <string>
namespace nar{
class SocketException
{
 public:
  SocketException ( std::string s ) : m_s ( s ) {};
  ~SocketException (){};

  std::string description() { return m_s; }

 private:

  std::string m_s;

};
}
#endif
