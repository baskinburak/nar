from subprocess import Popen, PIPE, STDOUT
import random
import string
import sys
import os

def make_directory(dirname):
    if not os.path.exists(dirname):
        os.makedirs(dirname)

def id_generator(size, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))
def createfile(filename):
    p = Popen(['touch', 'pushtest/'+filename], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate()[0]
    file = open('pushtest/'+filename, 'w')
    file.write(id_generator(random.randint(1000,2500000)))
    file.close()
    return stdout_data

def push(filename,username,password):
    p = Popen(['../../build/narcli','push',filename, '/'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate(input=username+'\n'+password+'\n')[0]
    return stdout_data

def deleteFiles():
    p = Popen(['rm',"-rf", 'pushtest/*'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate()[0]
    return stdout_data

if __name__=="__main__":
    i=22
    make_directory("pushtest")
    while(i<100):
        i+=1
        #createfile("filename"+str(i))

        print push("pushtest/filename"+str(i),"aa","aa")
    #deleteFiles()
    print "done"
