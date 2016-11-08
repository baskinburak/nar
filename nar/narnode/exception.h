#ifndef NAR_EXCEPTION_H
#define NAR_EXCEPTION_H
#include <stdexcept>
#include <string>

namespace nar {
    class Exception : public std::runtime_error {
        private:
        public:
            Exception(std::string m): std::runtime_error(m) {};
    };
}

#endif
