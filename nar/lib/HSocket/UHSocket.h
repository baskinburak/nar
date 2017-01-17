#ifndef NAR_THSOCKET_H
#define NAR_THSOCKET_H
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../json.hpp"
#include <vector>
#include <iterator>
#include <functional>
/*
I---->
O<----

*/
using namespace nlohmann;
using namespace boost::asio::ip;
namespace nar {
    class UHSocket {
        private:
            
        public:
            UHSocket() {
                
            }

            void introduce(std::string my_id, std::string server, std::string port) {
                
            }

            static void accept_handler(const boost::system::error_code& err) {
                
            }

            void wait() {
                
            }

            void establish(std::string peer_id) {
                
            }

            
    };
}

#endif
