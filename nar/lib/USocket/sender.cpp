#include "USocket.h"
int main() {
  nar::USocket sck(8712766U);
  sck.make_randevous(std::string("127.0.0.1"), 45877);
  return 0;
}
