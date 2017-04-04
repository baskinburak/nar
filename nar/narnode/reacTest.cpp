#include <iostream>
#include <nar/narnode/utility.h>
#include <nar/narnode/reactive.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <boost/asio.hpp>
#include <unistd.h>

using namespace nar;
using std::cout;
using std::endl;

boost::asio::io_service io;
Socket cli(io,'c');

void print_req(std::string keyword, std::string message) {
    cout << keyword.append("**********") << endl << message << endl << endl;
    return;
}


void chunk_push_req(int fSize) {
    MessageTypes::WaitChunkPush::Request req(10000,3,51,fSize);
    MessageTypes::WaitChunkPush::Response resp(200);
    req.send_mess(&cli,resp);

    print_req(std::string("ChunkPush"),std::string("GOT DAT"));
}

void keep_alive() {
    MessageTypes::KeepAlive::Request req1;
    MessageTypes::KeepAlive::Response resp1(200);

    std::string message = get_message(cli);
    print_req(std::string("KeepAlive"),message);

    req1.receive_message(Messaging::transform(message));
    resp1.send_mess(&cli);
    std::cout << "RESP SENT" << std::endl;
    return;
}

int main() {
    Socket server(io,'s');

    server.bind(20000);
    server.accept(cli);             // GET CONNECTION

    keep_alive();                   // KEEP ALIVE

    usleep(1000000);

    std::cout << "after counter" << std::endl;

    //chunk_push_req(625539);               // chunk push

    while(1);

    return 0;
}
