g++ U2CLIsender.cpp USocket.cpp Packet.cpp Socket.cpp ../../narnode/utility.cpp ../../narnode/File/File.cpp ../../lib/Cryption/aes.cpp ../../lib/Cryption/rsa.cpp -lcrypto++ -std=c++11 -pthread -lboost_system -lboost_random -lboost_iostreams -lboost_filesystem -o clisender -I../../../;
g++ U2CLIreceiver.cpp USocket.cpp Packet.cpp Socket.cpp ../../narnode/utility.cpp ../../narnode/File/File.cpp ../../lib/Cryption/aes.cpp ../../lib/Cryption/rsa.cpp -lcrypto++ -std=c++11 -pthread -lboost_system -lboost_random -lboost_iostreams -lboost_filesystem -o clireceiver -I../../../;
g++ U2SERV.cpp USocket.cpp Packet.cpp Socket.cpp ../../narnode/utility.cpp ../../narnode/File/File.cpp -std=c++11 ../../lib/Cryption/aes.cpp ../../lib/Cryption/rsa.cpp -lcrypto++ -pthread -lboost_system -lboost_random -lboost_iostreams -lboost_filesystem -o server -I../../../;
