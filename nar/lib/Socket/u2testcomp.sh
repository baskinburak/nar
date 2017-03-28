g++ U2CLIsender.cpp U2Socket.cpp Packet.cpp ../../narnode/File/File.cpp -std=c++11 -pthread -lboost_system -lboost_iostreams -lboost_filesystem -o clisender -I../../../;
g++ U2CLIreceiver.cpp U2Socket.cpp Packet.cpp ../../narnode/File/File.cpp -std=c++11 -pthread -lboost_system  -lboost_iostreams -lboost_filesystem -o clireceiver -I../../../;
g++ U2SERV.cpp U2Socket.cpp Packet.cpp ../../narnode/File/File.cpp -std=c++11 -pthread -lboost_system  -lboost_iostreams -lboost_filesystem -o server -I../../../;
