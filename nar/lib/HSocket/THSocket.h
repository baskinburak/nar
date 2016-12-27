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
    class THSocket {
        private:
            boost::asio::io_service io_service;
            tcp::socket socket;
            bool succeed;
            std::shared_ptr<tcp::socket> ptr;
        public:
            THSocket(): io_service(), socket(io_service), succeed(false) {
                socket.open(boost::asio::ip::tcp::v4());
                socket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            }

            void introduce(std::string my_id, std::string server, std::string port) {
                tcp::resolver resolver(io_service);
                tcp::resolver::query query(server, port);
                tcp::resolver::iterator endpoints = resolver.resolve(query);
                std::cout << "??" << std::endl;
                boost::asio::connect(socket, endpoints);
                std::cout << "!!" << std::endl;
                json req;
                req["i_am"] = my_id;
                std::string json_req = req.dump();
                boost::system::error_code err;

                boost::asio::write(socket, boost::asio::buffer(json_req, json_req.size()), err);

                boost::array<char, 2> reply;
                socket.read_some(boost::asio::buffer(reply));

                std::string repl;
                std::copy(reply.begin(), reply.end(), std::back_inserter(repl));

                if(repl == std::string("OK")) {
                    std::cout << "ok" << std::endl;
                }
            }

            static void accept_handler(const boost::system::error_code& err) {
                if(!err) {
                    std::cout << "accept succeed" << std::endl;
                } else {
                    std::cout << "accept failed." << std::endl;
                }
            }

            void wait() {
                std::cout << "nbr" << std::endl;
                boost::array<char, 128> reply;
                socket.read_some(boost::asio::buffer(reply));
                std::string repl;
                std::copy(reply.begin(), reply.end(), std::back_inserter(repl));

                json resp = json::parse(repl);
                std::string ip = resp["ip"];
                std::string port = resp["port"];

                tcp::resolver resolver(io_service);
                tcp::resolver::query query(ip, port);
                tcp::resolver::iterator endpoints = resolver.resolve(query);

                tcp::socket accepted_socket(io_service);
                accepted_socket.open(boost::asio::ip::tcp::v4());
                boost::system::error_code ec;
                accepted_socket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
                std::cout << ec << std::endl;
                std::cout << "nbr2" << std::endl;

                while(true) {
                    std::cout << "hop" << std::endl;
                    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), socket.local_endpoint().port()));
                    std::cout << "kop" << std::endl;
                    acceptor.async_accept(accepted_socket, &THSocket::accept_handler);
                    accepted_socket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                    if(succeed){ /*socket = accepted_socket;*/ break; }
                    boost::system::error_code ec;
                    std::cout << "nbrrrr" << std::endl;
                    boost::asio::connect(socket, endpoints, ec);std::cout << "gg" << std::endl;
                    if(!ec) break;
                }
                std::cout << "here i am " << std::endl;
            }

            void establish(std::string peer_id) {
                json req;
                req["remote"] = peer_id;
                boost::system::error_code err;

                std::string json_req = req.dump();

                boost::asio::write(socket, boost::asio::buffer(json_req, json_req.size()), err);
                boost::array<char, 128> reply;
                wait();
                
            }

            
    };
}

#endif
