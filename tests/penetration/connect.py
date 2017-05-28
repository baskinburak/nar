import socket

TCP_IP = "35.187.10.23"
TCP_PORT = 16670


def connect():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    return s
