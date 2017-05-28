import socket
import ssl
import pprint

TCP_IP = "35.187.10.23"
TCP_PORT = 16670



def connect():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.connect((TCP_IP, TCP_PORT))
    return s


def ssl_connect():    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    ssl_sock = ssl.wrap_socket(s,
                           ca_certs="server.crt",
                           cert_reqs=ssl.CERT_REQUIRED)
    ssl_sock.connect((TCP_IP, TCP_PORT))
    return ssl_sock
