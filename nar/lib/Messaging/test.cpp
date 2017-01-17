#include "MessageTypes/DirInfo.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/PeerConnectionRequest.h"
#include "MessageTypes/FilePullRequest.h"
#include <iostream>
int main() {
    /* DIRINFO TEST START */
    std::cout << "DIRINFO TEST START" << std::endl;
    nar::messagetypes::DirInfo::Request req("dirasdasd");
    std::cout << req.get_action() << std::endl << req.get_dir() << std::endl;


    nar::messagetypes::DirInfo::Response resp(200);
    resp.add_element(std::string("a"), std::string("b"), std::string("c"), 1, false);

    std::vector<nar::messagetypes::DirInfo::Response::DirListElement>& eles = resp.get_elements();
    std::cout << resp.get_statuscode() << " " << resp.get_replyto() << std::endl;
    for(int i=0; i<eles.size(); i++) {
        std::cout << eles[i].changetime << " " << eles[i].entityid << " " << eles[i].entityname << " " << eles[i].entitysize << " " << eles[i].type << std::endl;
    }
    std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    /* FILEPUSHREQEUST TEST START*/
    std::cout << "FILEPUSHREQEUST TEST START" << std::endl;
    nar::messagetypes::FilePushRequest::Request reqfile(std::string("yark"), std::string("/"), 12314123);
    std::cout << reqfile.get_action() << " " << reqfile.get_filename() << " " << reqfile.get_dir() << " " << reqfile.get_filesize() << std::endl;

    nar::messagetypes::FilePushRequest::Response respfile(200, std::string("fileidasdasf"), 1234);
    respfile.add_element(std::string("peerid1231"), std::string("chunkid"), std::string("token123124"));

    std::vector<nar::messagetypes::FilePushRequest::Response::PeerListElement>& elesfile = respfile.get_elements();
    std::cout << respfile.get_statuscode() << " " << respfile.get_replyto() << " " << respfile.get_fileid() << " " << respfile.get_chunksize() << std::endl;
    for(int i=0; i<elesfile.size(); i++) {
        std::cout << elesfile[i].peerid << " " << elesfile[i].chunkid << " " << elesfile[i].token << std::endl;
    }
    std::cout << "FILEPUSHREQUEST TEST END" << std::endl << std::endl;


    /* KEEPALIVE TEST START*/
    std::cout << "KEEPALIVE TEST START" << std::endl;
    nar::messagetypes::KeepAlive::Request req4;
    std::cout << req4.get_action() << std::endl;

    nar::messagetypes::KeepAlive::Response resp4(200);
    std::cout << resp4.get_statuscode() << std::endl;
    std::cout << resp4.get_replyto() << std::endl;
    std::cout << "KEEPALIVE TEST END" << std::endl << std::endl;
    /* KEEPALIVE TEST END */


    /* PEERCONNECTIONREQUEST TEST START */
    std::cout << "PEERCONNECTIONREQUEST TEST START" << std::endl;
    nar::messagetypes::PeerConnectionRequest::Request req55("peerid12345");
    std::cout << req55.get_action() << std::endl << req55.get_peer_id() << std::endl;

    nar::messagetypes::PeerConnectionRequest::Response resp55(200);
    resp55.add_element(std::string("peerip127001"), 23456, std::string("peerId123"));

    std::vector<nar::messagetypes::PeerConnectionRequest::Response::PeerListElement>& eles55 = resp55.get_elements();
    std::cout << resp55.get_statuscode() << " " << resp55.get_replyto() << std::endl;
    for(int i=0; i<eles55.size(); i++) {
        std::cout << eles55[i].peer_ip << " " << eles55[i].peer_port << " " << eles55[i].peer_id << std::endl;
    }
    std::cout << "PEERCONNECTIONREQUEST TEST END" << std::endl << std::endl;
    /* PEERCONNECTIONREQUEST TEST END */


    /* FILEPULLREQEUST TEST START*/
    std::cout << "FILEPULLREQEUST TEST START" << std::endl;
    nar::messagetypes::FilePullRequest::Request reqfile56(std::string("yark"), std::string("/"));
    std::cout << reqfile56.get_action() << " " << reqfile56.get_filename() << " " << reqfile56.get_dir() << std::endl;

    nar::messagetypes::FilePullRequest::Response respfile56(200, std::string("fileidasdasf"), 1234);
    respfile56.add_element(std::string("peerid1231"), std::string("chunkid"), std::string("token123124"));

    std::vector<nar::messagetypes::FilePullRequest::Response::PeerListElement>& elesfile56 = respfile56.get_elements();
    std::cout << respfile56.get_statuscode() << " " << respfile56.get_replyto() << " " << respfile56.get_fileid() << " " << respfile56.get_chunksize() << std::endl;
    for(int i=0; i<elesfile56.size(); i++) {
        std::cout << elesfile56[i].peerid << " " << elesfile56[i].chunkid << " " << elesfile56[i].token << std::endl;
    }
    std::cout << "FILEPULLREQUEST TEST END" << std::endl << std::endl;


    return 0;
}
