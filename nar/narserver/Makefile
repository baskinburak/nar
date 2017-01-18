SRC_FILES := $(shell find ../../  ! -name "randevous_server.cpp" ! -name "receiver.cpp" ! -name "sender.cpp" ! -name "cryptor.cpp" ! -name "aes.cpp" ! -name "test.cpp" ! -name "server.cpp" ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp" ! -name "compressor.cpp")

all: narserver

narserver: *.cpp
	g++ -std=c++11 -o narserver server.cpp $(SRC_FILES) -I../../ -lpthread -lmysqlcppconn -lcrypto++ -lz

clean:
	rm -f narserver
