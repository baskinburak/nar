SRC_FILES := $(shell find ../../  ! -name "randevous_server.cpp" ! -name "receiver.cpp" ! -name "sender.cpp" ! -name "aes.cpp" ! -name "test.cpp" ! -name "Database.cpp" ! -name "Database2.cpp" ! -name "server.cpp" ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "cryptor.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp" ! -name "compressor.cpp")

all: nardaemon narcli

nardaemon: *.cpp
	g++ -std=c++11 -o nardaemon daemon.cpp $(SRC_FILES) -I../../ -lpthread -lcrypto++ -lz

narcli: *.cpp
	g++ -std=c++11 -o narcli cli.cpp $(SRC_FILES) -I../../ -lpthread -lcrypto++ -lz

clean:
	rm -f nardaemon narcli
