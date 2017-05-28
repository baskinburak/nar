from subprocess import Popen, PIPE, STDOUT
def ls(username,password):

    p = Popen(['../../build/narcli','ls'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = p.communicate(input=username+'\n'+password+'\n')[0]
    return stdout_data

if __name__=="__main__":
    i=0
    while(i<100):
        i+=1
        print ls("aa","aa")
    print "done"
