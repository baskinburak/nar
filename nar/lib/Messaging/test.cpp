#include "MessageTypes/DirInfo.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/Register.h"
#include "MessageTypes/Handshake.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/PeerPortPushRequest.h"
#include "MessageTypes/InfoChunkPush.h"
#include "MessageTypes/InfoChunkPull.h"
#include "MessageTypes/ChunkReceiveRequest.h"
#include <iostream>
using namespace nar::messagetypes;
int main() {
    /* DIRINFO TEST START */
    std::cout << "DIRINFO TEST START" << std::endl;
    DirInfo::Request req1("dirasdasd");
    std::cout << req1.get_action() << std::endl << req1.get_dir() << std::endl;

    DirInfo::Response resp1(200);
    resp1.add_element(std::string("a"), std::string("b"), std::string("c"), 1, false);
    std::vector<DirInfo::Response::DirListElement>& eles = resp1.get_elements();
    std::cout << resp1.get_statuscode() << " " << resp1.get_replyto() << std::endl;
    for(int i=0; i<eles.size(); i++) {
        std::cout << eles[i].changetime << " " << eles[i].entityid << " " << eles[i].entityname << " " << eles[i].entitysize << " " << eles[i].type << std::endl;
    }
    std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    /* FILEPUSHREQEUST TEST START */
    std::cout << "FILEPUSHREQEUST TEST START" << std::endl;
    FilePushRequest::Request req2(std::string("yark"), std::string("/"), 12314123);
    std::cout << req2.get_action() << " " << req2.get_filename() << " " << req2.get_dir() << " " << req2.get_filesize() << std::endl;

    FilePushRequest::Response resp2(200, std::string("fileidasdasf"), 1234);
    resp2.add_element(std::string("peerid1231"), std::string("chunkid"), std::string("token123124"));

    std::vector<FilePushRequest::Response::PeerListElement>& elesfile = resp2.get_elements();
    std::cout << resp2.get_statuscode() << " " << resp2.get_replyto() << " " << resp2.get_fileid() << " " << resp2.get_chunksize() << std::endl;
    for(int i=0; i<elesfile.size(); i++) {
        std::cout << elesfile[i].peerid << " " << elesfile[i].chunkid << " " << elesfile[i].token << std::endl;
    }
    std::cout << "FILEPUSHREQUEST TEST END" << std::endl << std::endl;
    /* FILEPUSHREQEUST TEST START */

    /* REGISTER TEST  */
    std::cout<< "REGISTER TEST START "<<std::endl;
    Register::Request req3(std::string("insan"),std::string("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbcccccccccccc"));
    std::cout<<"type "<<req3.get_action()<<" username "<<req3.get_username()<< " aes "<<req3.get_aes()<<std::endl;

    Register::Response res3(200);
    std::cout<<"type "<<res3.get_replyto()<<" statuscode "<<res3.get_statuscode()<<std::endl;
    std::cout<< "REGISTER TEST END "<<std::endl<<std::endl;
    /* REGISTER TEST END */

    /* HANDSHAKE TEST START */
	std::cout << "HANDSHAKE TEST START" << std::endl;
	Handshake::Request req5("userNAME");
	std::cout << req5.get_username() << std::endl;


	Handshake::Response resp5(300);
	std::cout << resp5.get_statuscode() << " " << resp5.get_replyto() << std::endl;
    	std::cout << "HANDSHAKE TEST END" << std::endl;
    /* HANDSHAKE TEST END */




    /* KEEPALIVE TEST START*/
    std::cout << "KEEPALIVE TEST START" << std::endl;
    KeepAlive::Request req4;
    std::cout << req4.get_action() << std::endl;

    KeepAlive::Response resp4(200);
    std::cout << resp4.get_statuscode() << std::endl;
    std::cout << resp4.get_replyto() << std::endl;
    std::cout << "KEEPALIVE TEST END" << std::endl;
    /* KEEPALIVE TEST END */


	/*PUSHFILE TESTS START */
    std::cout << "PUSHFILE TEST START" << std::endl;
	// 7-8
    std::cout << "7-8 TEST START" << std::endl;
	PeerPortPushRequest::Request reqpush7("tokenasdasd123");
	std::cout << reqpush7.get_token() << " " << reqpush7.get_action() << std::endl;

	PeerPortPushRequest::Response resppush8(350,65554);
    std::cout << resppush8.get_port() << std::endl;
    std::cout << resppush8.get_statuscode() << std::endl;
    std::cout << resppush8.get_replyto() << std::endl;
    std::cout << "7-8 TEST END" << std::endl;

    std::cout << "14-15 TEST START" << std::endl;
	InfoChunkPush::Request reqpush14(1112323423, false);
	std::cout << reqpush14.get_chunkId() << " " << reqpush14.get_success() << " "<< reqpush14.get_action() << std::endl;

	InfoChunkPush::Response resppush15(888);
    std::cout << resppush15.get_statuscode() << std::endl;
    std::cout << resppush15.get_replyto() << std::endl;
    std::cout << "14-15 TEST END" << std::endl;


    std::cout << "PUSHFILE TEST END" << std::endl;
	/*PUSHFILE TESTS END */

	/*PULLFILE TESTS START */
    std::cout << "PULLFILE TEST START" << std::endl;
	// 7-8
    std::cout << "10-11 TEST START" << std::endl;
	ChunkReceiveRequest::Request reqpull10("tokenasdasd123", 123123123, 2000);
	std::cout << reqpull10.get_token() << " " << reqpull10.get_chunkId() << " " << reqpull10.get_chunkSize() << std::endl;

	ChunkReceiveRequest::Response resppull11(350);
    std::cout << resppull11.get_statuscode() << std::endl;
    std::cout << resppull11.get_replyto() << std::endl;
    std::cout << "10-11 TEST END" << std::endl;

    std::cout << "15-16 TEST START" << std::endl;
	InfoChunkPull::Request reqpull15(1112323423, true);
	std::cout << reqpull15.get_chunkId() << " " << reqpull15.get_success() << " "<< reqpull15.get_action() << std::endl;

	InfoChunkPull::Response resppull16(888);
    std::cout << resppull16.get_statuscode() << std::endl;
    std::cout << resppull16.get_replyto() << std::endl;
    std::cout << "15-16 TEST END" << std::endl;


    std::cout << "PULLFILE TEST END" << std::endl;
	/*PULLFILE TESTS END */




    return 0;
}
