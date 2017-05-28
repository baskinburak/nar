import sys
import os
sys_dir = os.path.split(os.getcwd())[0]
util_path = os.path.join(sys_dir,"util")
sys.path.append(util_path)

import pull
import pushtest
import register
import lstest

if __name__ == "__main__":
	count = 3
	test_file_name = "test"

	username,password = str(raw_input("Username Password <")).split()
	#register.run_daemon()
	pushtest.push_test(username,password,"pushFolder", test_file_name,count)
	print lstest.ls(username,password)
	
	pullFolder = pushtest.make_directory("pullFolder")
	pushFolder = pushtest.make_directory("pushFolder")
	for i in range(1,count):
		fName = test_file_name + str(i)
		pull.pull(username,password,fName, pullFolder)
		pullHash = pull.get_md5(pullFolder+"/"+fName)
		pushHash = pull.get_md5(pushFolder+"/"+fName)
		if pullHash == pushHash :
			print "File " + fName + " passed"
		else :
			print "Hash failed for file " +fName

	print "Test Finished"
	






