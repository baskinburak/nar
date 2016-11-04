SRC_FILES := $(shell find . ! -name "cli.cpp" -name "*.cpp")

all: nardaemon narcli

nardaemon: *.cpp
	g++ -o nardaemon $(SRC_FILES) -I../../

narcli: *.cpp
	g++ -o narcli cli.cpp -I../../

clean:
	rm -f nardaemon narcli
