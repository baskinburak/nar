#include "USocket.h"
#include <fstream>
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


  sck.send((char*)str.c_str(), str.size());

  std::cout << "going to infinite loop" << std::endl;
  while(true);
  return 0;
}
