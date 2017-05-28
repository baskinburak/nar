from subprocess import Popen, PIPE, STDOUT
import random
import string
import sys
import os

def make_directory(dirname):
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    return os.path.abspath(dirname)

def id_generator(size, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))
def createfile(dirName,filename):
    p = Popen(['touch', dirName+'/'+filename], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate()[0]
    file = open(dirName+'/'+filename, 'w')
    file.write(id_generator(random.randint(1000,2500000)))
    file.close()
    return stdout_data

def push(filename,username,password):
    print filename
    p = Popen(['../../../build/narcli','push',filename, '/'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate(input=username+'\n'+password+'\n')[0]
    return stdout_data

def deleteFiles(dirName):
    p = Popen(['rm',"-rf", dirName+'/*'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate()[0]
    return stdout_data

def push_test(username,password,dirName,fileName,pushCount):
    i=0
    wtf = make_directory(dirName)
    while(i<pushCount):
        i+=1
        createfile(wtf,fileName+str(i))
        print push(wtf+"/"+fileName +str(i),username,password)
    #deleteFiles()
    print "done"

if __name__=="__main__":
    i=22
    make_directory("pushtest")
    while(i<100):
        i+=1
        #createfile("filename"+str(i))

        print push("pushtest/filename"+str(i),"aa","aa")
    #deleteFiles()
    print "done"
