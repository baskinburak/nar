#include "logger.h"
#include <iostream>

int main() {

	nar::Logger a(std::cout);
	std::string asd("asdf");
	a << "asdf" << asd << std::endl;
return 0;
}
