import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 54321))
(str, addr) = s.recvfrom(1024)
print addr
(str, addr) = s.recvfrom(1024)
print addr
