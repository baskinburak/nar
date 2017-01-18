#include "USocket.h"
int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cout << "give me compliments" << std::endl;
  }
  nar::USocket sck(8712766U);
  sck.make_randevous(std::string(argv[0]), 45877);

  sck.send("yarak naber amk", 15); 
  return 0;
}
