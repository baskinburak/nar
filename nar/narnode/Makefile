SRC_FILES := $(shell find . ! -name "cli.cpp" ! -name "daemon.cpp" ! -name "FileKeeperTest.cpp" -name "*.cpp")

all: nardaemon narcli

nardaemon: *.cpp
	g++ -std=c++11 -o nardaemon daemon.cpp $(SRC_FILES) -I../../ -lpthread

narcli: *.cpp
	g++ -std=c++11 -o narcli cli.cpp $(SRC_FILES) -I../../ -lpthread

clean:
	rm -f nardaemon narcli
