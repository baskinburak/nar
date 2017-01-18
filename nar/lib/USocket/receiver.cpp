#include "USocket.h"
int main() {
  if(argc != 2) {
    std::cout << "give me compliments" << std::endl;
  }
  nar::USocket sck(8712766U);
  sck.make_randevous(std::string(argv[0]), 45877);

  char buf[129];
  sck.recv(buf, 15);
  std::cout << buf << std::endl;

  while(true);
  return 0;
}
