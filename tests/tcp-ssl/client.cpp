#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
int main() {

    boost::asio::io_service ioserv;

    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("server.crt");

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_sck(ioserv, ctx);

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 17171);

    boost::asio::ip::tcp::socket::lowest_layer_type& sock = ssl_sck.lowest_layer();

    sock.connect(endpoint);
    sock.set_option(boost::asio::ip::tcp::no_delay(true));

    ssl_sck.set_verify_mode(boost::asio::ssl::verify_peer);
    ssl_sck.set_verify_callback(boost::asio::ssl::rfc2818_verification("127.0.0.1"));

    ssl_sck.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::client);
    return 0;
}
