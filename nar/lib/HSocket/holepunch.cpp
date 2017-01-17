#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <algorithm>

int main() {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("localhost", 12345);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    boost::array<char, 128> buf;
    boost::system::error_code err;

    while(true) {
        size_t len = socket.read_some(boost::asio::buffer(buf), err);

        if(err == boost::asio::error::eof) {
            std::cout << "connection closed by remote" << std::endl;
        }

        std::cout.write(buf.data(), len);
    }
}
