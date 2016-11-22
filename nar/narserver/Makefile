SRC_FILES := $(shell find ../../ ! -name "server.cpp" ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp")

all: narserver

narserver: *.cpp
	g++ -std=c++11 -o narserver server.cpp $(SRC_FILES) -I../../ -lpthread

clean:
	rm -f narserver
