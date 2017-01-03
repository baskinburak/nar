#include "MessageTypes/ChunkSendRequest.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/DirInfo.h"
#include "MessageTypes/PeerPortRequest.h"
#include <iostream>
#include "MessageTypes/WaitChunkPullRequest.h"
#include "MessageTypes/Register.h"
#include "MessageTypes/DirInfo.h"
//#include "MessageTypes/MessageRS.h"

//using namespace nar::messagetypes;
//using namespace nar;
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
        resp.sendMessage();
        std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    /* FILEPUSHREQEUST TEST START*/
        std::cout << "FILEPUSHREQEUST TEST START" << std::endl;
        nar::messagetypes::FilePushRequest::Request push5(std::string("yark"), std::string("/"), 12314123);
        std::cout << push5.get_action() << " " << push5.get_filename() << " " << push5.get_dir() << " " << push5.get_filesize() << std::endl;
        nar::messagetypes::FilePushRequest::Request req2(std::string("yark"), std::string("/"), 12314123);
        std::cout << req2.get_action() << " " << req2.get_filename() << " " << req2.get_dir() << " " << req2.get_filesize() << std::endl;
        push5.sendMessage();
        nar::messagetypes::FilePushRequest::Response push6(200, std::string("fileidasdasf"), 1234);
        push6.add_element(std::string("peerid1231"), std::string("chunkid"), std::string("token123124"));
        push6.sendMessage();
        nar::messagetypes::FilePushRequest::Response push6_2(200, std::string("fileidasdasf"), 1234);
        push6_2.add_element(std::string("peerid1231"), std::string("chunkid"), std::string("token123124"));
        push6_2.add_element(std::string("peasdıugakj"), std::string("chunaakid"), std::string("tokenaa123124"));

        /*std::vector<nar::messagetypes::FilePushRequest::Response::PeerListElement>& elesfile = push6.get_elements();
        std::cout << push6.get_statuscode() << " " << push6.get_replyto() << " " << push6.get_fileid() << " " << push6.get_chunksize() << std::endl;
*/
        std::vector<nar::messagetypes::FilePushRequest::Response::PeerListElement>& elesfile = push6_2.get_elements();
        std::cout << push6_2.get_statuscode() << " " << push6_2.get_replyto() << " " << push6_2.get_fileid() << " " << push6_2.get_chunksize() << std::endl;
        for(int i=0; i<elesfile.size(); i++) {
            std::cout << elesfile[i].peerid << " " << elesfile[i].chunkid << " " << elesfile[i].token << std::endl;
        }
        push6_2.sendMessage();

        std::cout << "FILEPUSHREQUEST TEST END" << std::endl << std::endl;
    /* REGISTER TEST  */
    std::cout<< "REGISTER TEST START "<<std::endl;
    nar::messagetypes::Register::Request req3(std::string("insan"),std::string("aaaaaaaaaaaaaaaaaaaaabbbbbbbbbcccccccccccc"));
    std::cout<<"type "<<req3.get_action()<<" username "<<req3.get_username()<< " aes "<<req3.get_aes()<<std::endl;
    req3.sendMessage();
    std::cout<< "REGISTER TEST END "<<std::endl<<std::endl;
    /* REGISTER TEST END */

    /* HANDSHAKE TEST START *//*
	std::cout << "HANDSHAKE TEST START" << std::endl;
	Handshake::Request req5("userNAME");
	std::cout << req5.get_username() << std::endl;



    /* KEEPALIVE TEST START*//*
    std::cout << "KEEPALIVE TEST START" << std::endl;
    KeepAlive::Request req4;
    std::cout << req4.get_action() << std::endl;
    /* KEEPALIVE TEST END */


    /* WAITCHUNKPUSHREQUEST TEST START *//*
    std::cout<<"WAITCHUNKPUSHREQUEST TEST START "<<std::endl;
    WaitChunkPushRequest::Request push3(std::string("token"),3,100);
    std::cout<<"type "<<push3.get_action()<<" token "<<push3.get_token()<<" chunk_id "<<push3.get_chunk_id()<<" chunk_size "<<push3.get_chunk_size()<<std::endl;
    /* WAITCHUNKPUSHREQUEST TEST END */


    /* CHUNKSENDREQUEST TEST START */
    std::cout<<"CHUNKSENDREQUEST TEST START "<<std::endl;
    nar::messagetypes::ChunkSendRequest::Request push10(std::string("token"),3,100);
    std::cout<<"type "<<push10.get_action()<<" token "<<push10.get_token()<<" chunk_id "<<push10.get_chunk_id()<<" chunk_size "<<push10.get_chunk_size()<<std::endl;
    push10.sendMessage();
    nar::messagetypes::ChunkSendRequest::Response push11(200);
    //std::cout<<"type "<<push10.get_action()<<" token "<<push10.get_token()<<" chunk_id "<<push10.get_chunk_id()<<" chunk_size "<<push10.get_chunk_size()<<std::endl;
    push11.sendMessage();
    /* CHUNKSENDREQUEST TEST END */


    /* PEERPORTREQUEST TEST START *//*
    std::cout<<"PEERPORTREQUEST TEST START "<<std::endl;
    PeerPortRequest::Request pull7;
    std::cout<<"type "<<pull7.get_action()<<std::endl;
    /* PEERPORTREQUEST TEST END */


    /* WAITCHUNKPULLREQUEST TEST START *//*
    std::cout<<"WAITCHUNKPULLREQUEST TEST START "<<std::endl;
    WaitChunkPullRequest::Request pull3(std::string("token"),3,100);
    std::cout<<"type "<<pull3.get_action()<<" token "<<pull3.get_token()<<" chunk_id "<<pull3.get_chunk_id()<<" chunk_size "<<pull3.get_chunk_size()<<std::endl;
    std::cout<<"WAITCHUNKPULLREQUEST TEST END "<<std::endl<<std::endl;
    /* WAITCHUNKPULLREQUEST TEST END */




    /* MESSAGERS SEND TEST START*//*
    std::vector<SendMessObject> rsTest;


    SendMessObject head;
    SendMessObject pay;
    SendMessElement ele1;
    SendMessElement ele2;
    SendMessElement ele3;
    SendMessElement ele4;


    SendMessObject obj1;
    SendMessObject obj2;


    std::vector<SendMessObject> mtObject;



    std::vector<::SendMessElement> objEle;




    ele1.name = std::string("action");
    ele1.type = (SendMessTypes)2;
    std::string a =std::string("dir_info");
    ele1.var = &a;



    ele2.name = std::string("peer_id");
    ele2.type = (SendMessTypes)0;
    int b = 12345;
    ele2.var = &b;

    std::vector<::SendMessElement>  c;
    c.push_back(ele1);
    c.push_back(ele2);

    ele3.name = std::string("eleArr");
    ele3.type = (SendMessTypes)3;
    ele3.var = &c;
    std::cout << (*(std::vector<SendMessElement> *)ele3.var).size() << std::endl;


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
    ele4.type = (SendMessTypes)4;
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
