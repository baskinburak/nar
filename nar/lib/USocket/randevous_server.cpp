#include "USocket.h"
int main() {
  nar::USocket randevous(0U);
  std::cout << randevous.get_port() << std::endl;
  randevous.randevous_server();
  return 0;
}
