#include "MessageTypes/DirInfo.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/Register.h"
#include "MessageTypes/Handshake.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/WaitChunkPushRequest.h"
#include "MessageTypes/ChunkSendRequest.h"
#include "MessageTypes/PeerPortRequest.h"
#include "MessageTypes/WaitChunkPullRequest.h"
#include "MessageTypes/MessageRS.h"
#include <iostream>
using namespace nar;
int main() {
    /* DIRINFO TEST START *//*
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


    /* FILEPUSHREQEUST TEST START *//*
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

    /* REGISTER TEST  *//*
    std::cout<< "REGISTER TEST START "<<std::endl;
    Register::Request req3(std::string("insan"),std::string("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbcccccccccccc"));
    std::cout<<"type "<<req3.get_action()<<" username "<<req3.get_username()<< " aes "<<req3.get_aes()<<std::endl;

    Register::Response res3(200);
    std::cout<<"type "<<res3.get_replyto()<<" statuscode "<<res3.get_statuscode()<<std::endl;
    std::cout<< "REGISTER TEST END "<<std::endl<<std::endl;
    /* REGISTER TEST END */

    /* HANDSHAKE TEST START *//*
	std::cout << "HANDSHAKE TEST START" << std::endl;
	Handshake::Request req5("userNAME");
	std::cout << req5.get_username() << std::endl;


	Handshake::Response resp5(300);
	std::cout << resp5.get_statuscode() << " " << resp5.get_replyto() << std::endl;
    	std::cout << "HANDSHAKE TEST END" << std::endl;
    /* HANDSHAKE TEST END */




    /* KEEPALIVE TEST START*//*
    std::cout << "KEEPALIVE TEST START" << std::endl;
    KeepAlive::Request req4;
    std::cout << req4.get_action() << std::endl;

    KeepAlive::Response resp4(200);
    std::cout << resp4.get_statuscode() << std::endl;
    std::cout << resp4.get_replyto() << std::endl;
    std::cout << "KEEPALIVE TEST END" << std::endl<<std::endl;
    /* KEEPALIVE TEST END */


    /* WAITCHUNKPUSHREQUEST TEST START *//*
    std::cout<<"WAITCHUNKPUSHREQUEST TEST START "<<std::endl;
    WaitChunkPushRequest::Request push3(std::string("token"),3,100);
    std::cout<<"type "<<push3.get_action()<<" token "<<push3.get_token()<<" chunk_id "<<push3.get_chunk_id()<<" chunk_size "<<push3.get_chunk_size()<<std::endl;

    WaitChunkPushRequest::Response push4(200);
    std::cout<<"type "<<push4.get_replyto()<<" statuscode "<<push4.get_statuscode()<<std::endl;
    std::cout<<"WAITCHUNKPUSHREQUEST TEST END "<<std::endl<<std::endl;
    /* WAITCHUNKPUSHREQUEST TEST END */


    /* CHUNKSENDREQUEST TEST START *//*
    std::cout<<"CHUNKSENDREQUEST TEST START "<<std::endl;
    ChunkSendRequest::Request push10(std::string("token"),3,100);
    std::cout<<"type "<<push10.get_action()<<" token "<<push10.get_token()<<" chunk_id "<<push10.get_chunk_id()<<" chunk_size "<<push10.get_chunk_size()<<std::endl;

    ChunkSendRequest::Response push11(200);
    std::cout<<"type "<<push11.get_replyto()<<" statuscode "<<push11.get_statuscode()<<std::endl;
    std::cout<<"CHUNKSENDREQUEST TEST END "<<std::endl<<std::endl;
    /* CHUNKSENDREQUEST TEST END */


    /* PEERPORTREQUEST TEST START *//*
    std::cout<<"PEERPORTREQUEST TEST START "<<std::endl;
    PeerPortRequest::Request pull7;
    std::cout<<"type "<<pull7.get_action()<<std::endl;

    PeerPortRequest::Response pull8(200,12345);
    std::cout<<"type "<<pull8.get_replyto()<<" statuscode "<<pull8.get_statuscode()<<" port_number "<<pull8.get_port_number()<<std::endl;
    std::cout<<"PEERPORTREQUEST TEST END "<<std::endl<<std::endl;
    /* PEERPORTREQUEST TEST END */


    /* WAITCHUNKPULLREQUEST TEST START *//*
    std::cout<<"WAITCHUNKPULLREQUEST TEST START "<<std::endl;
    WaitChunkPullRequest::Request pull3(std::string("token"),3,100);
    std::cout<<"type "<<pull3.get_action()<<" token "<<pull3.get_token()<<" chunk_id "<<pull3.get_chunk_id()<<" chunk_size "<<pull3.get_chunk_size()<<std::endl;

    WaitChunkPullRequest::Response pull4(200);
    std::cout<<"type "<<pull4.get_replyto()<<" statuscode "<<pull4.get_statuscode()<<std::endl;
    std::cout<<"WAITCHUNKPULLREQUEST TEST END "<<std::endl<<std::endl;
    /* WAITCHUNKPULLREQUEST TEST END */




    /* MESSAGERS SEND TEST START*/
    std::vector<MessObject> rsTest;


    MessObject head;
    MessObject pay;
    MessElement ele1;
    MessElement ele2;
    MessElement ele3;
    MessElement ele4;


    MessObject obj1;
    MessObject obj2;


    std::vector<MessObject> mtObject;



    std::vector<::MessElement> objEle;




    ele1.name = std::string("action");
    ele1.type = (MessTypes)2;
    std::string a =std::string("dir_info");
    ele1.var = &a;



    ele2.name = std::string("peer_id");
    ele2.type = (MessTypes)0;
    int b = 12345;
    ele2.var = &b;

    std::vector<::MessElement>  c;
    c.push_back(ele1);
    c.push_back(ele2);

    ele3.name = std::string("eleArr");
    ele3.type = (MessTypes)3;
    ele3.var = &c;
    std::cout << (*(std::vector<MessElement> *)ele3.var).size() << std::endl;


    objEle.push_back(ele1);
    objEle.push_back(ele2);

    obj1.name = std::string("obj1 name");
    obj1.var = objEle;
    obj1.size = 2;


    obj2.name = std::string("obj2 name");
    obj2.var = objEle;
    obj2.size = 2;


    mtObject.push_back(obj1);
    mtObject.push_back(obj2);

    ele4.name = std::string("objArr");
    ele4.type = (MessTypes)4;
    ele4.var = &mtObject;



    head.name = std::string("header");
    head.size = 1;
    head.var.push_back(ele1);



    pay.name = std::string("payload");
    pay.size = 3;
    pay.var.push_back(ele2);
    pay.var.push_back(ele3);
    pay.var.push_back(ele4);



    rsTest.push_back(head);
    rsTest.push_back(pay);
    ::sendMessage(rsTest);

    /* MESSAGERS SEND TEST END*/

    return 0;
}
