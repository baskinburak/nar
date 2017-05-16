from subprocess import call
import os

def fu1_fu2(nar_dir):					#nar/lib/Cryption/aes.cpp
	crypt_dir=os.path.join(nar_dir,"lib","Cryption")
	aes_dir = os.path.join(crypt_dir,"aes.cpp")
	rsa_dir = os.path.join(crypt_dir,"rsa.cpp")
	call(["g++","aes_rsa.cpp",aes_dir,rsa_dir,"-o","aes","-I"+os.path.split(nar_dir)[0],"-lboost_random","-lcryptopp",])
	call(["./aes"])

def fu3(nar_dir):
	base_dir = os.path.join(nar_dir, "lib","base64","base64.cpp")
	call(["g++", "base64.cpp",base_dir, "-o", "base", "-I"+os.path.split(nar_dir)[0] ])
	call(["./base"])

if __name__ == "__main__":
	cwd = os.getcwd()
	nar_dir = os.path.join(os.path.split(os.path.split(cwd)[0])[0],"nar")

	fu1_fu2(nar_dir)
	fu3(nar_dir)
	
