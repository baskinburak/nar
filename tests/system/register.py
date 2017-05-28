from subprocess import Popen, PIPE, STDOUT

def register(username,password):	
	p = Popen(['sudo', '../../build/nardaemon'],stdout=PIPE, stdin=PIPE, stderr=PIPE)
	inp =  "n" + "\n" + username + "\n" + password + "\n" + "1\n" + "1\n" 
	p.stdin.write(inp)
	print "Registered with username: " + username + " password: " + password 

def run_daemon():
	p = Popen(['sudo', '../../build/nardaemon'],stdout=PIPE, stdin=PIPE, stderr=PIPE)
	
if __name__ == "__main__":
	run_daemon()
