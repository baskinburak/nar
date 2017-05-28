import sys
sys.path.append("..")


import connect

from os import listdir
from os.path import isfile, join

files = [f for f in listdir("messages/") if isfile(join("messages/", f))]

files = sorted(files)

for fil in files:
    fil = "messages/" + fil
    s = connect.connect()
    hand = open(fil, "r")
    content = hand.readlines()
    hand.close()
    print "Testing #" + fil
    for lin in content:
        s.send(lin)
        s.recv(1024)
    s.close()
    s = connect.connect()
    print "PASS"
    s.close()
