from subprocess import Popen, PIPE, STDOUT

def pull(username,password,file_name,path):	
	p = Popen(['../../../build/narcli', 'pull', file_name, path],stdout=PIPE, stdin=PIPE, stderr=PIPE)
	inp = username + "\n" + password + "\n"
	stdout_data = p.communicate(input=inp)[0]
	print "%s named file pulled to the directory %s" % (file_name, path)

def get_md5(file_path):
	p = Popen(['md5sum',file_path],stdout=PIPE, stdin=PIPE, stderr=PIPE)
	stdout_data = p.communicate()[0]
	return stdout_data.split()[0]

if __name__ == "__main__":
	pull("utku","123","register.py","try/")
