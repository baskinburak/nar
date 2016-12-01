SRC_FILES := $(shell find ../../ ! -name "aes.cpp" ! -name "test.cpp" ! -name "Database.cpp" ! -name "Database2.cpp" ! -name "server.cpp" ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "cryptor.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp")

all: nardaemon narcli

nardaemon: *.cpp
	g++ -std=c++11 -o nardaemon daemon.cpp $(SRC_FILES) -I../../ -lpthread -lcrypto++

narcli: *.cpp
	g++ -std=c++11 -o narcli cli.cpp $(SRC_FILES) -I../../ -lpthread -lcrypto++

clean:
	rm -f nardaemon narcli
