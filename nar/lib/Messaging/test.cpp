#include "MessageTypes/DirInfo.h"
#include "MessageTypes/Handshake.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/UserRegister.h"
#include "MessageTypes/MachineRegister.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/FilePullRequest.h"
#include "MessageTypes/WaitChunkPushRequest.h"
#include "MessageTypes/WaitChunkPullRequest.h"
#include "MessageTypes/InfoChunkPull.h"
#include "MessageTypes/InfoChunkPush.h"

#include <iostream>

using namespace nar::messagetypes;


int main() {
    /*INFOCHUNKPUSH TEST START*/
        std::cout << "INFOCHUNKPUSH TEST START" << std::endl<<std::endl;
        std::cout << "INFOCHUNKPUSH TEST END" << std::endl<<std::endl;
    /*INFOCHUNKPUSH TEST END*/

    /*INFOCHUNKPULL TEST START*/
        std::cout << "INFOCHUNKPULL TEST START" << std::endl<<std::endl;
        std::cout << "INFOCHUNKPULL TEST END" << std::endl<<std::endl;
    /*INFOCHUNKPULL TEST END*/

    /*WAITCHUNKPULLREQUEST TEST START*/
        std::cout << "WAITCHUNKPULLREQUEST TEST START" << std::endl<<std::endl;
        std::cout << "WAITCHUNKPULLREQUEST TEST END" << std::endl<<std::endl;
    /*WAITCHUNKPULLREQUEST TEST END*/

    /*WAITCHUNKPUSHREQUEST TEST START*/
        std::cout << "WAITCHUNKPUSHREQUEST TEST START" << std::endl<<std::endl;
        std::cout << "WAITCHUNKPUSHREQUEST TEST END" << std::endl<<std::endl;
    /*WAITCHUNKPUSHREQUEST TEST END*/

    /*FILEPULLREQUEST TEST START*/
        std::cout << "FILEPULLREQUEST TEST START" << std::endl<<std::endl;
        std::cout << "FILEPULLREQUEST TEST END" << std::endl<<std::endl;
    /*FILEPULLREQUEST TEST END*/

    /*FILEPUSHREQUEST TEST START*/
        std::cout << "FILEPUSHREQUEST TEST START" << std::endl<<std::endl;
        std::cout << "FILEPUSHREQUEST TEST END" << std::endl<<std::endl;
    /*FILEPUSHREQUEST TEST END*/

    /*MACHINEREGISTER TEST START*/
        std::cout << "MACHINEREGISTER TEST START" << std::endl<<std::endl;
        std::cout << "MACHINEREGISTER TEST END" << std::endl<<std::endl;
    /*MACHINEREGISTER TEST END*/

    /*USERREGISTER TEST START*/
        std::cout << "USERREGISTER TEST START" << std::endl<<std::endl;
        std::cout << "USERREGISTER TEST END" << std::endl<<std::endl;
    /*USERREGISTER TEST END*/


    /*KEEPALIVE TEST START*/
        std::cout << "KEEPALIVE TEST START" << std::endl<<std::endl;
        std::cout << "KEEPALIVE TEST END" << std::endl<<std::endl;
    /*KEEPALIVE TEST END*/

    /*HANDSHAKE TEST START*/
        std::cout << "HANDSHAKE TEST START" << std::endl<<std::endl;
        std::cout << "HANDSHAKE TEST END" << std::endl<<std::endl;
    /*HANDSHAKE TEST END*/





    /* DIRINFO TEST START */
        std::cout << "DIRINFO TEST START" << std::endl<<std::endl;

        DirInfo::Request dir_req("dir_reqasdasd");
        std::cout << dir_req.get_action() << std::endl << dir_req.get_dir() << std::endl;
        std::cout<<"dir request send Test"<<std::endl;
		dir_req.sendMessage();
        std::cout<<"dir request receive Test"<<std::endl;
        DirInfo::Request dir_req2("OOOOO");
        dir_req2.receiveMessage(dir_req.test_json());
        dir_req2.sendMessage();


        DirInfo::Response dir_resp(200);
        dir_resp.add_element(std::string("a"), std::string("b"), std::string("c"), 1, false);
        dir_resp.add_element(std::string("k"), std::string("l"), std::string("m"), 2, true);

        std::vector<DirInfo::Response::DirListElement>& eles = dir_resp.get_elements();
        std::cout << dir_resp.get_statuscode() << " " << dir_resp.get_replyto() << std::endl;
        for(int i=0; i<eles.size(); i++) {
            std::cout << eles[i].change_time << " " << eles[i].entity_id << " " << eles[i].entity_name << " " << eles[i].entity_size << " " << eles[i].type << std::endl;
        }
        std::cout<<"dir response send Test"<<std::endl;
        dir_resp.sendMessage();
        std::cout<<"dir response receive Test"<<std::endl;
        DirInfo::Response dir_resp2(200);
        dir_resp2.receiveMessage(dir_resp.test_json());
        dir_resp2.sendMessage();

        std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    return 0;
}
