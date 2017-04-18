#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
int main() {


    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.set_default_verify_paths();

    ctx.use_certificate_chain_file("server.crt"); 
    ctx.use_private_key_file("server.key", boost::asio::ssl::context::pem);
    ctx.use_tmp_dh_file("dh512.pem");

    boost::asio::io_service ioserv;

    boost::asio::ip::tcp::acceptor accp(ioserv);


    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_sck(ioserv, ctx);

    boost::asio::ip::tcp::socket::lowest_layer_type& sck = ssl_sck.lowest_layer();

    accp.accept(sck);


    ssl_sck.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::server);

    
    return 0;
}
