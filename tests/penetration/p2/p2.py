print "SSL Handshake Protocol Test"

import sys
sys.path.append("..")


import connect

from os import listdir
from os.path import isfile, join

files = [f for f in listdir("messages/") if isfile(join("messages/", f))]

files = sorted(files)
i = 1
for fil in files:
    res = str(i)
    i+=1
    fil = "messages/" + fil
    s = connect.connect()
    hand = open(fil, "r")
    content = hand.read().splitlines()
    hand.close()
    print "Testing #" + fil
    for lin in content:
        s.sendall(lin)
        s.recv(1024)
    s.close()
    s = connect.connect()
    res += " PASS"
    s.close()
    print res
