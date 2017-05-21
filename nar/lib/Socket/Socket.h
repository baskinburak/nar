#ifndef NAR_SOCKET_H
#define NAR_SOCKET_H

#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <nar/lib/Exception/Exception.h>

namespace nar {

    /**
    * Class which provides the interface and services for tcp-ip socket operations
    *
    * @author: utku
    * @privar: _type, char, holds the type of the socket( server or client socket)
    * @privar: _sock, boost::asio::ip::tcp::socket, holds the boost tcp-ip socket in the case of initialization as 'c' (client) socket
    * @pubvar: _acceptor, boost::asio::ip::tcp::acceptor, holds the tcp-ip listener-acceptor structure in the case of initialization as 's' (server) socket
    * @tested: yes (except pre socket binding scenarios)
    * @todo: CORRECTING*** pre interface, port binding and connectiong and accepting afterwards
    **/

    class Socket
    {
        public:
            /**
            * Constructor of a tcp-ip socket
            * @author: utku
            * @param: io_serv, boost::asio::io_service, io service to be provided for sharing of the resources of the platform.
            * @param: type, char, Socket type. c for client socket and s for server socket.
            * @tested: yes
            * @todo: -
            **/
            Socket(boost::asio::io_service& io_serv, boost::asio::ssl::context& ctx, char type); //"s", "c"

            /**
            * Destructor of a tcp-ip socket
            * @author: utku
            * @tested: yes
            * @todo: -
            **/
            ~Socket();

            /**
            * Binds a socket to an available port provided by OS.
            * @author: utku
            * @tested: yes
            * @todo: -
            **/
            void bind();

            /**
            * Binds the socket to the specified port.
            * @author: utku
            * @param: port, const unsigned short, port
            * @tested: yes
            * @todo: -
            **/
            void bind(const unsigned short port);

            /**
            * Binds the socket to the specified port and interface.
            * @author: utku
            * @param: phort, const unsigned short, port
            * @param: interface, const char*, interface as "127.0.0.1"
            * @tested: yes
            * @todo: -
            **/
            void bind(const unsigned short port, const char* interface);


            /**
            * Provides the service of accepting connections for server sockets. New connection is
            * directed to the socket specified in argument.
            *
            * @author: utku
            * @param: acc, nar::Socket, Socket to which the new connection will be redirected. A socket that is initiliazed as
            * client is sufficient as an argument
            *
            * @tested: yes
            * @todo: -
            **/
            void accept(Socket& acc) const;


            /**
            * Provides the service to connect to a remote endpoint with client sockets.
            * @author: utku
            * @param: host, std::string, ip adress of the server
            * @param: port, const unsigned short, port of the server
            * @tested: yes
            * @todo: !!! Current condition of the connect doesnt work when one tries to bind to a specific port and/or interface and than tries to conenct to a remote end point.
            * To provide this service implementation must be changed using boost::asio::connect.
            **/
            void connect(const std::string& host, const unsigned short port);

            /**
            * Send data from a client socket
            * @author: utku
            * @param: offset, const char*, pointer which points to the data to be sent.
            * @param: length, int, how many bytes to be sent.
            * @tested: yes
            * @todo: -
            **/
            void send(const char* offset, int length) const;
            /**
            * Send data from a client socket
            * @author: dogu
            * @param: offset, const char*, pointer which points to the data to be sent.
            * @param: length, int, how many bytes to be sent.
            * @tested: yes
            * @todo: -
            **/
            void send(std::string s) const;

            /**
            * Receives data from a client socket.
            * @author: utku
            * @param: offset, char*, pointer to the buffer to which data will be written.
            * @param: length, int, this argument specifies how many bytes at max. is intended to received.
            * return:  int, returns the number of bytes it managed to receive. This number may be smaller than the number specified by parameter2.
            * @tested: yes
            * @todo: -
            **/
            int recv(char* offset, int length) const;

            /**
            * Closes the socket and by freeing file descriptor and port used. It is a good practice close the sockets after using.
            * @author: utku
            * @tested: no
            * @todo: -
            **/
            void close() const;

            void forceclose() const;

            /**
            * This function is used to learn to which ip the socket is connected.
            * @author: utku
            * @tested: yes
            * @todo: -
            **/
            std::string get_dest_ip() const;

            /**
            * This function is used to learn to which local port the socket is bound.
            * @author: utku
            * @tested: yes
            * return : unsigned short, port.
            * @todo: -
            **/
            unsigned short get_local_port() const;

        private:
            char _type;
            boost::asio::ip::tcp::acceptor* _acceptor;
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* _ssl_sock;
    };
}

#endif
