import sys
sys.path.append("..")

print "Low Level Messaging Attack"

import connect

from os import listdir
from os.path import isfile, join

files = [f for f in listdir("messages/") if isfile(join("messages/", f))]

files = sorted(files)

i = 1
for fil in files:
    res = " PASS"
    fil = "messages/" + fil
    s = connect.ssl_connect()
    hand = open(fil, "r")
    content = hand.read().splitlines()
    hand.close()
    print str(i),
    sys.stdout.flush()
    for lin in content:
        s.write(lin)
        try:
            s.read(1024)
        except Exception:
            res = " FAIL"
    s.close()
    s = connect.ssl_connect()
    s.close()
    print res
    i+=1
