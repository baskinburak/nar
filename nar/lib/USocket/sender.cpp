#include "USocket.h"
#include <fstream>
#include <cstring>
int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cout << "give me compliments" << std::endl;
    return 0;
  }
  nar::USocket sck(8712766U);
  std::cout << sck.get_port() << std::endl;
  sck.make_randevous(std::string(argv[1]), 45877);

std::ifstream t(argv[2]);
std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());


  int total = 0;
  char* buf = new char[str.size()+100];
  memcpy(buf, str.c_str(), str.size());

  while(total != 18378) {
    total += sck.send(buf + total, std::min(1024, 18378-total));
  }
  sck.stop_thread = true;
  std::cout << "going to infinite loop" << std::endl;
  while(true);
  return 0;
}
