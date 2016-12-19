#include "MessageTypes/Handshake.h"
#include <iostream>
using namespace nar::messagetypes;
int main() {
    /* DIRINFO TEST START */
   /* std::cout << "DIRINFO TEST START" << std::endl;
    DirInfo::Request req("dirasdasd");
    std::cout << req.get_action() << std::endl << req.get_dir() << std::endl;


    DirInfo::Response resp(200);
    resp.add_element(std::string("a"), std::string("b"), std::string("c"), 1, false);

    std::vector<nar::messagetypes::DirInfo::Response::DirListElement>& eles = resp.get_elements();
    std::cout << resp.get_statuscode() << " " << resp.get_replyto() << std::endl;
    for(int i=0; i<eles.size(); i++) {
        std::cout << eles[i].changetime << " " << eles[i].entityid << " " << eles[i].entityname << " " << eles[i].entitysize << " " << eles[i].type << std::endl;
    }
    std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    /* FILEPUSHREQEUST TEST START
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
    std::cout << "FILEPUSHREQUEST TEST END" << std::endl << std::endl;*/
    
	std::cout << "HANDSHAKE TEST START" << std::endl;
	Handshake::Request req5("userNAME");
	std::cout << req5.get_username() << std::endl;


	Handshake::Response resp5(300);
	std::cout << resp5.get_statuscode() << " " << resp5.get_replyto() << std::endl;

    return 0;
}
