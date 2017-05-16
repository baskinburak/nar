#include <ostream>
#include <string>
#include <iostream>
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
	static void printPass() {
	    Modifier green(Color::FG_GREEN);
	    Modifier def(Color::FG_DEFAULT);
	    std::cout << green << "------PASSED!------"<< def << std::endl;	
	}
	static void printFail() {
	    Modifier green(Color::FG_RED);
	    Modifier def(Color::FG_DEFAULT);
	    std::cout << green << "------FAILED!------"<< def << std::endl;	
	}
    };
}
