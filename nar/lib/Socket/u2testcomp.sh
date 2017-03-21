g++ U2CLI.cpp U2Socket.cpp Packet.cpp -std=c++11 -pthread -lboost_system -o cli -I../../../;
g++ U2SERV.cpp U2Socket.cpp Packet.cpp -std=c++11 -pthread -lboost_system -o srv -I../../../;
