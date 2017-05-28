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
    res = " PASS"
    print str(i),
    sys.stdout.flush()
    s = connect.connect()
    fil = "messages/" + fil
    hand = open(fil, "r")
    content = hand.read().splitlines()
    hand.close()
    for lin in content:
        s.sendall(lin)
        try:
            s.recv(1024)
        except Exception:
            res = " FAIL"
    s.close()
    s = connect.connect()
    s.close()
    print res
    i+=1
