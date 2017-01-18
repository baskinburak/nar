#include "USocket.h"
int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cout << "give me compliments" << std::endl;
  }
  nar::USocket sck(8712766U);
  std::cout << sck.get_port() << std::endl;

  sck.make_randevous(std::string(argv[1]), 45877);

  char buf[50000];
  int total = 0;
  while(total != 17306) {
    int len = sck.recv(buf, std::min(1024, 17306-total));
    std::string etci(buf, len);
    std::cout << etci << std::endl;
    total+=len;
  }
  std::cout << "going to infinite loop" << std::endl;
  while(true);
  return 0;
}
