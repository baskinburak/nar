#include <nar/lib/base64/base64.h>
#include "colormod.h"
#include <iostream>
#include <sstream>

//std::string base64_encode(unsigned char const* , unsigned int len);
//std::string base64_decode(std::string const& s);

int main() {
	std::cout << "testing " << "FU3" << " ..." << std::endl;
	std::stringstream ss;
	std::string a("Corum dunyanin merkezidir");
	ss << "String to encode: \n" << a << std::endl;
	std::string res = base64_encode( (unsigned char const *) a.c_str() , a.length());
	ss << "Encoded string: \n" << res << std::endl;
	ss << res << std::endl;
	std::string res2 = base64_decode(res);
	ss << "Decoded string \n" << res2 << std::endl;
	
	if (res2 == a) {
		Color::Modifier::printPass();
	} else {
		Color::Modifier::printFail();
		std::cout << ss.str() << std::endl;
	}

	return 0;
}
