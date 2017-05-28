import sys
sys.path.append("..")


import connect

from os import listdir
from os.path import isfile, join

files = [f for f in listdir("messages/") if isfile(join("messages/", f))]

files = sorted(files)


for fil in files:
    fil = "messages/" + fil
    s = connect.ssl_connect()
    hand = open(fil, "r")
    content = hand.read().splitlines()
    hand.close()
    print "Testing #" + fil
    for lin in content:
        s.write(lin)
        s.write(lin)
        s.write(lin)
        print s.read(1024)
    s.close()
    s = connect.ssl_connect()
    print "PASS"
    s.close()
