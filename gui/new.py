import easygui as g
import sys
from subprocess import Popen, PIPE

pathToCli = "/home/fatih/bitirme/nar/build/narcli"

class User(object):
	def __init__(self,username,password):
		self.uname = username
		self.pw = str(password)

class Command(object):
	def __init__(self, user ,args = []):
		self.cmd = pathToCli
		self.args = args
		self.user = user
	def exe(self):
		"""
		Execute the external command and get its exitcode, stdout and stderr.
		"""
		print [self.cmd]+self.args
		proc = Popen([self.cmd]+self.args, stdin=PIPE,stdout=PIPE)
		#out, err = proc.communicate()
		proc.stdin.write(self.user.uname + '\n')
		proc.stdin.write(self.user.pw + '\n' )
		proc.stdout.read(20)				# Read Username: Password: part
		return proc

class Ls(Command):
	def __init__(self,user,dirName = None):
		if dirName:
			Command.__init__(self,user,['ls',dirName])
		else:
			Command.__init__(self,user,['ls',])
	def exe(self):
		proc = Command.exe(self)
		
		inp = []
		line = proc.stdout.readline()
		while line:
			inp.append(line)
			line = proc.stdout.readline()
		proc.stdin.close()
		return inp

class Push(Command):
	def __init__(self,user,fileName = None):
		Command.__init__(self,user,['push',fileName])
	def exe(self):
		proc = Command.exe(self)
		
		inp = []
		line = proc.stdout.readline()
		while line:
			inp.append(line)
			line = proc.stdout.readline()
		proc.stdin.close()
		return inp

class Pull(Command):
	def __init__(self,user,fileName, dirName):
		Command.__init__(self,user,['pull',fileName,dirName])
	def exe(self):
		proc = Command.exe(self)
		
		inp = []
		line = proc.stdout.readline()
		while line:
			inp.append(line)
			line = proc.stdout.readline()
		proc.stdin.close()
		return inp			

def setUser():
	uname = g.enterbox("Enter Your Username","Username","")
	if not uname:
		sys.exit(0)
	pw = g.passwordbox(msg='Enter your password', title='Password', default='')
	if not pw:
		sys.exit(0)
		
	return User(uname,pw)



if __name__ == "__main__":
	user = setUser()
	g.msgbox("Hello " + user.uname + " with pw " + user.pw)

	msg   = "Welcome To Nar " + user.uname
	#choices = ["Ls","Quit"]
	msg = []
	while True:
		#reply=g.buttonbox(msg,choices=choices)		
		"""if str(reply) == "Ls":
			
		elif str(reply) == "Quit":
			sys.exit(1) """ 
		ls = Ls(user)
		msg = ls.exe()
		pp = g.buttonbox("".join(msg), title='Welcome To Nar', choices=("Upload File","Download File"))
		
		if pp == "Upload File":
			userFile = g.fileopenbox(msg="File to Upload", title="Nar")
			if not userFile:
				continue
			else:
				print userFile
				pu = Push(user,userFile)
				pu.exe()
				continue
		elif pp == "Download File":
			choice = g.buttonbox("Please select a file", title='Welcome To Nar', choices=tuple(msg))
			fName = choice.split()[0][3:]
			dirName = g.diropenbox(title="Save to")
			print fName, dirName
			pu = Pull(user,fName,dirName)
			pu.exe()
			continue
			


	
