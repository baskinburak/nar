import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 12346))
dict = {}
s.listen(5)

(s1, addr) = s.accept()
data = s1.recv(1024)
import json
j = json.loads(data)
dict[j['i_am']] = (s1,addr)
s1.send("OK")
print addr

(s2, addr) = s.accept()
data = s2.recv(1024)
j = json.loads(data)
dict[j['i_am']] = (s2,addr)
s2.send("OK")
print addr

data = s1.recv(1024)
j = json.loads(data)
necmi = dict["necmi"]
baskin = dict["baskin"]

if(j["remote"] == "necmi"):
	necmi_send = json.dumps({'ip': str(baskin[1][0]), 'port' : str(baskin[1][1])})
	baskin_send = json.dumps({'ip': str(necmi[1][0]), 'port': str(necmi[1][1])})
	s1.send(baskin_send)
	s2.send(necmi_send)
else:
	pass

raw_input()
