g++ -std=c++11 randevous_server.cpp Packet.cpp USocket.cpp -o randevous_server -lpthread;
g++ -std=c++11 sender.cpp Packet.cpp USocket.cpp -o sender -lpthread;
g++ -std=c++11 receiver.cpp Packet.cpp USocket.cpp -o receiver -lpthread;
