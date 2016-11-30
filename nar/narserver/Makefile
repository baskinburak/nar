SRC_FILES := $(shell find ../../ ! -name "aes.cpp" ! -name "test.cpp" ! -name "server.cpp" ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp")

all: narserver

narserver: *.cpp
	g++ -std=c++11 -o narserver server.cpp $(SRC_FILES) -I../../ -lpthread -lmysqlcppconn

clean:
	rm -f narserver
