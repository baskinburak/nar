import paramiko
import thread
import sys
reclone = False
pull = False
if(len(sys.argv) > 1):
    if(sys.argv[1] == "redeploy"):
        reclone = True
    if(sys.argv[1] == "pull"):
        pull = True
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect("35.187.10.23", username="doguyeke", key_filename="/root/.ssh/id_rsa")
print "Killing..."
stdin, stdout, stderr = ssh.exec_command('pkill -f gitlab.ceng.metu.edu.tr')
exit_status = stdout.channel.recv_exit_status() 
stdin, stdout, stderr = ssh.exec_command('pkill -f narserver')
exit_status = stdout.channel.recv_exit_status() 
if(reclone):
    print "Deleting..."
    stdin, stdout, stderr = ssh.exec_command('rm -rf nar')
    exit_status = stdout.channel.recv_exit_status()
    print "Opening tunnel..."
    stdin, stdout, stderr = ssh.exec_command('ssh -p 8085 -f -L3333:gitlab.ceng.metu.edu.tr:22 e1942697@external.ceng.metu.edu.tr -N')
    exit_status = stdout.channel.recv_exit_status() 
    print "Cloning devel..."
    stdin, stdout, stderr = ssh.exec_command('git clone ssh://git@localhost:3333/RecklessUncivilPeople/nar.git')
    for line in iter(lambda: stdout.readline(2048), ""):
        sys.stdout.write(line)
        sys.stdout.flush()
    exit_status = stdout.channel.recv_exit_status()
    print "Reinit db..."
    stdin, stdout, stderr = ssh.exec_command('cat /home/doguyeke/nar/nar/narserver/database_dump/lastDb.sql')
    res = stdout.read()
    exit_status = stdout.channel.recv_exit_status()
    stdin, stdout, stderr = ssh.exec_command('mysql -u root -p nar')
    stdin.write("123\n")
    stdin.write(res)
    stdin.flush()
    exit_status = stdout.channel.recv_exit_status()
    print "CMake & Make..."
    stdin, stdout, stderr = ssh.exec_command('cd nar; mkdir build; cd build; cmake ..; make narserver')
    for line in iter(lambda: stdout.readline(2048), ""):
        sys.stdout.write(line)
        sys.stdout.flush()
    exit_status = stdout.channel.recv_exit_status()
if(pull):
    print "Opening tunnel..."
    stdin, stdout, stderr = ssh.exec_command('ssh -p 8085 -f -L3333:gitlab.ceng.metu.edu.tr:22 e1942697@external.ceng.metu.edu.tr -N')
    exit_status = stdout.channel.recv_exit_status() 
    print "Pull..."
    stdin, stdout, stderr = ssh.exec_command('cd nar; git pull')
    stdin.write("123\n")
    exit_status = stdout.channel.recv_exit_status()
    print "Reinit db..."
    stdin, stdout, stderr = ssh.exec_command('cat /home/doguyeke/nar/nar/narserver/database_dump/lastDb.sql')
    res = stdout.read()
    exit_status = stdout.channel.recv_exit_status()
    stdin, stdout, stderr = ssh.exec_command('mysql -u root -p nar')
    stdin.write("123\n")
    stdin.write(res)
    stdin.flush()
    exit_status = stdout.channel.recv_exit_status()
    print "Make..."
    stdin, stdout, stderr = ssh.exec_command('cd nar; mkdir build; cd build; make narserver')
    for line in iter(lambda: stdout.readline(2048), ""):
        sys.stdout.write(line)
        sys.stdout.flush()
    exit_status = stdout.channel.recv_exit_status()
print "Run..."
stdin, stdout, stderr = ssh.exec_command('./nar/build/narserver')
for line in iter(lambda: stdout.readline(2048), ""):
    sys.stdout.write(line)
    sys.stdout.flush()
exit_status = stdout.channel.recv_exit_status() 

