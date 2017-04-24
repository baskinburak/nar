#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <iostream>

std::string get_password() {
    return std::string("h4rd4tt4ck");
}

int main() {


    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);

    ctx.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);


   // ctx.set_password_callback(&get_password);

    ctx.use_certificate_chain_file("server.crt"); 
    ctx.use_private_key_file("server.key", boost::asio::ssl::context::pem);
    ctx.use_tmp_dh_file("dh512.pem");

    boost::asio::io_service ioserv;

    boost::asio::ip::tcp::acceptor accp(ioserv, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 17171));


    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_sck(ioserv, ctx);

    boost::asio::ip::tcp::socket::lowest_layer_type& sck = ssl_sck.lowest_layer();

    accp.accept(sck);


    ssl_sck.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::server);
    
    boost::system::error_code ec;
    char buf[1024];
    ssl_sck.read_some(boost::asio::buffer(buf,14), ec);
    

    std::cout << buf << std::endl;
    return 0;
}
