#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <algorithm>

int main() {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(tcp::v4(), 12345));

    boost::asio::ip::tcp::socket f_sck(io_service);
    acceptor.accept(f_sck);

    boost::asio::ip::tcp::socket s_sck(io_service);
    acceptor.accept(s_sck);

    boost::asio::ip::tcp::endpoint f_endpoint = f_sck.remote_endpoint();
    boost::asio::ip::tcp::endpoint s_endpoint = s_sck.remote_endpoint();

    boost::asio::ip::address f_address = f_end
}
