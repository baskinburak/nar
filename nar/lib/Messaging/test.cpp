<<<<<<< HEAD
#include "MessageTypes/DirInfo.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/Register.h"
=======
#include "MessageTypes/Handshake.h"
>>>>>>> origin/utku
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


    return 0;
}
