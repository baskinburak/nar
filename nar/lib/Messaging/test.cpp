#include "MessageTypes/DirInfo.h"
#include "MessageTypes/Handshake.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/UserRegister.h"
#include "MessageTypes/MachineRegister.h"
#include "MessageTypes/FilePush.h"
#include "MessageTypes/FilePull.h"
#include "MessageTypes/WaitChunkPush.h"
#include "MessageTypes/WaitChunkPull.h"
#include "MessageTypes/InfoChunkPull.h"
#include "MessageTypes/InfoChunkPush.h"

#include <iostream>

using namespace nar::MessageTypes;


int main() {
    /*INFOCHUNKPUSH TEST START*/
        std::cout << "INFOCHUNKPUSH TEST START" << std::endl<<std::endl;

        InfoChunkPush::Request ipush_req1(2,false);
        std::cout<<"chunk_id "<<ipush_req1.get_chunk_id()<<" success "<<ipush_req1.get_success()<<std::endl;
        std::cout<<"InfoChunkPush request send Test"<<std::endl;
        ipush_req1.send_mess();
        std::cout<<"InfoChunkPush request receive Test"<<std::endl;
        InfoChunkPush::Request ipush_req2(3,true);
        ipush_req2.receive_message(ipush_req1.test_json());
        ipush_req2.send_mess();

        InfoChunkPush::Response ipush_resp1(200);
        std::cout<<"InfoChunkPush response send Test"<<std::endl;
        ipush_resp1.send_mess();
        std::cout<<"InfoChunkPush response receive Test"<<std::endl;
        InfoChunkPush::Response ipush_resp2(300);
        ipush_resp2.receive_message(ipush_resp1.test_json());
        ipush_resp2.send_mess();


        std::cout << "INFOCHUNKPUSH TEST END" << std::endl<<std::endl;
    /*INFOCHUNKPUSH TEST END*/

    /*INFOCHUNKPULL TEST START*/
        std::cout << "INFOCHUNKPULL TEST START" << std::endl<<std::endl;

        InfoChunkPull::Request ipull_req1(2,false);
        std::cout<<"chunk_id "<<ipull_req1.get_chunk_id()<<" success "<<ipull_req1.get_success()<<std::endl;
        std::cout<<"InfoChunkPull request send Test"<<std::endl;
        ipull_req1.send_mess();
        std::cout<<"InfoChunkPull request receive Test"<<std::endl;
        InfoChunkPull::Request ipull_req2(3,true);
        ipull_req2.receive_message(ipull_req1.test_json());
        ipull_req2.send_mess();

        InfoChunkPull::Response ipull_resp1(200);
        std::cout<<"InfoChunkPull response send Test"<<std::endl;
        ipull_resp1.send_mess();
        std::cout<<"InfoChunkPull response receive Test"<<std::endl;
        InfoChunkPull::Response ipull_resp2(300);
        ipull_resp2.receive_message(ipull_resp1.test_json());
        ipull_resp2.send_mess();

        std::cout << "INFOCHUNKPULL TEST END" << std::endl<<std::endl;
    /*INFOCHUNKPULL TEST END*/

    /*WAITCHUNKPULL TEST START*/
        std::cout << "WAITCHUNKPULL TEST START" << std::endl<<std::endl;

        WaitChunkPull::Request wpull_req1(12345,"aaaaaaaaa",222222,333333);
        std::cout<<"stream_id "<<wpull_req1.get_stream_id()<<" randevous_port "<<wpull_req1.get_randevous_port()<<" chunk_id "<<wpull_req1.get_chunk_id()<<" chunk_size "<<wpull_req1.get_chunk_size()<<std::endl;
        std::cout<<"WaitChunkPull request send Test"<<std::endl;
        wpull_req1.send_mess();
        std::cout<<"WaitChunkPull request receive Test"<<std::endl;
        WaitChunkPull::Request wpull_req2(44444,"bbbbbbbbb",444444,555555);
        wpull_req2.receive_message(wpull_req1.test_json());
        wpull_req2.send_mess();

        WaitChunkPull::Response wpull_resp1(200);
        std::cout<<"WaitChunkPull response send Test"<<std::endl;
        wpull_resp1.send_mess();
        std::cout<<"WaitChunkPull response receive Test"<<std::endl;
        WaitChunkPull::Response wpull_resp2(300);
        wpull_resp2.receive_message(wpull_resp1.test_json());
        wpull_resp2.send_mess();

        std::cout << "WAITCHUNKPULL TEST END" << std::endl<<std::endl;
    /*WAITCHUNKPULL TEST END*/

    /*WAITCHUNKPUSH TEST START*/
        std::cout << "WAITCHUNKPUSH TEST START" << std::endl<<std::endl;

        WaitChunkPush::Request wpush_req1(12345,"aaaaaaaaa",222222,333333);
        std::cout<<"stream_id "<<wpush_req1.get_stream_id()<<" randevous_port "<<wpush_req1.get_randevous_port()<<" chunk_id "<<wpush_req1.get_chunk_id()<<" chunk_size "<<wpush_req1.get_chunk_size()<<std::endl;
        std::cout<<"WaitChunkPush request send Test"<<std::endl;
        wpush_req1.send_mess();
        std::cout<<"WaitChunkPush request receive Test"<<std::endl;
        WaitChunkPush::Request wpush_req2(44444,"bbbbbbbbb",444444,555555);
        wpush_req2.receive_message(wpush_req1.test_json());
        wpush_req2.send_mess();

        WaitChunkPush::Response wpush_resp1(200);
        std::cout<<"WaitChunkPush response send Test"<<std::endl;
        wpush_resp1.send_mess();
        std::cout<<"WaitChunkPush response receive Test"<<std::endl;
        WaitChunkPush::Response wpush_resp2(300);
        wpush_resp2.receive_message(wpush_resp1.test_json());
        wpush_resp2.send_mess();

        std::cout << "WAITCHUNKPUSH TEST END" << std::endl<<std::endl;
    /*WAITCHUNKPUSH TEST END*/

    /*FILEPULL TEST START*/
        std::cout << "FILEPULL TEST START" << std::endl<<std::endl;

        FilePull::Request fpull_req1("dir1","file1");
        std::cout<<"dir "<<fpull_req1.get_dir()<<" filename "<<fpull_req1.get_filename()<<std::endl;
        std::cout<<"FilePull request send Test"<<std::endl;
        fpull_req1.send_mess();
        std::cout<<"FilePull request receive Test"<<std::endl;
        FilePull::Request fpull_req2("dir2","file2");
        fpull_req2.receive_message(fpull_req1.test_json());
        fpull_req2.send_mess();

        FilePull::Response fpull_resp1(200,12345);
        fpull_resp1.add_element("mymac1",11111,"s1",40000);
        fpull_resp1.add_element("mymac2",22222,"s2",50000);
        std::cout<<"FilePull response send Test"<<std::endl;
        fpull_resp1.send_mess();
        std::cout<<"FilePull response receive Test"<<std::endl;
        FilePull::Response fpull_resp2(300,33333);
        fpull_resp2.receive_message(fpull_resp1.test_json());
        fpull_resp2.send_mess();

        std::cout << "FILEPULL TEST END" << std::endl<<std::endl;
    /*FILEPULL TEST END*/

    /*FILEPUSH TEST START*/
        std::cout << "FILEPUSH TEST START" << std::endl<<std::endl;

        FilePush::Request fpush_req1("dir1","file1",30000);
        std::cout<<"dir "<<fpush_req1.get_dir()<<" filename "<<fpush_req1.get_filename()<<" filesize "<<fpush_req1.get_filesize()<<std::endl;
        std::cout<<"FilePush request send Test"<<std::endl;
        fpush_req1.send_mess();
        std::cout<<"FilePush request receive Test"<<std::endl;
        FilePush::Request fpush_req2("dir2","file2",40000);
        fpush_req2.receive_message(fpush_req1.test_json());
        fpush_req2.send_mess();

        FilePush::Response fpush_resp1(200,12345);
        fpush_resp1.add_element("mymac1",11111,"s1",40000);
        fpush_resp1.add_element("mymac2",22222,"s2",50000);
        std::cout<<"FilePush response send Test"<<std::endl;
        fpush_resp1.send_mess();
        std::cout<<"FilePush response receive Test"<<std::endl;
        FilePush::Response fpush_resp2(300,33333);
        fpush_resp2.receive_message(fpush_resp1.test_json());
        fpush_resp2.send_mess();


        std::cout << "FILEPUSH TEST END" << std::endl<<std::endl;
    /*FILEPUSH TEST END*/

    /*MACHINEREGISTER TEST START*/
        std::cout << "MACHINEREGISTER TEST START" << std::endl<<std::endl;

        MachineRegister::Request macreg_req1;
        std::cout<<"MachineRegister request send Test"<<std::endl;
        macreg_req1.send_mess();
        std::cout<<"MachineRegister request receive Test"<<std::endl;
        MachineRegister::Request macreg_req2;
        macreg_req2.receive_message(macreg_req1.test_json());
        macreg_req2.send_mess();

        MachineRegister::Response macreg_resp1(200,"mac1");
        std::cout<<"machine_id "<<macreg_resp1.get_machine_id()<<std::endl;
        std::cout<<"MachineRegister response send Test"<<std::endl;
        macreg_resp1.send_mess();
        std::cout<<"MachineRegister response receive Test"<<std::endl;
        MachineRegister::Response macreg_resp2(300,"mac2");
        macreg_resp2.receive_message(macreg_resp1.test_json());
        macreg_resp2.send_mess();

        std::cout << "MACHINEREGISTER TEST END" << std::endl<<std::endl;
    /*MACHINEREGISTER TEST END*/

    /*USERREGISTER TEST START*/
        std::cout << "USERREGISTER TEST START" << std::endl<<std::endl;

        UserRegister::Request ureg_req1("fatih","aaaaaaaaaaaaa");
        std::cout<<"username "<<ureg_req1.get_username()<<" aes "<<ureg_req1.get_aes()<<std::endl;
        std::cout<<"UserRegister request send Test"<<std::endl;
        ureg_req1.send_mess();
        std::cout<<"UserRegister request receive Test"<<std::endl;
        UserRegister::Request ureg_req2("utku","bbbbbbbbbbbbbb");
        ureg_req2.receive_message(ureg_req1.test_json());
        ureg_req2.send_mess();

        UserRegister::Response ureg_resp1(200);
        std::cout<<"UserRegister response send Test"<<std::endl;
        ureg_resp1.send_mess();
        std::cout<<"UserRegister response receive Test"<<std::endl;
        UserRegister::Response ureg_resp2(300);
        ureg_resp2.receive_message(ureg_resp1.test_json());
        ureg_resp2.send_mess();


        std::cout << "USERREGISTER TEST END" << std::endl<<std::endl;
    /*USERREGISTER TEST END*/


    /*KEEPALIVE TEST START*/
        std::cout << "KEEPALIVE TEST START" << std::endl<<std::endl;

        KeepAlive::Request keep_req1;
        std::cout<<"KeepAlive request send Test"<<std::endl;
        keep_req1.send_mess();
        std::cout<<"KeepAlive request receive Test"<<std::endl;
        KeepAlive::Request keep_req2;
        keep_req2.receive_message(keep_req1.test_json());
        keep_req2.send_mess();

        KeepAlive::Response keep_resp1(200);
        std::cout<<"KeepAlive response send Test"<<std::endl;
        keep_resp1.send_mess();
        std::cout<<"KeepAlive response receive Test"<<std::endl;
        KeepAlive::Response keep_resp2(300);
        keep_resp2.receive_message(keep_resp1.test_json());
        keep_resp2.send_mess();

        std::cout << "KEEPALIVE TEST END" << std::endl<<std::endl;
    /*KEEPALIVE TEST END*/

    /*HANDSHAKE TEST START*/
        std::cout << "HANDSHAKE TEST START" << std::endl<<std::endl;

        Handshake::Request hand_req1("fatih","mac1");
        std::cout<<"username "<<hand_req1.get_username()<<" machine_id "<<hand_req1.get_machine_id()<<std::endl;
        std::cout<<"Handshake request send Test"<<std::endl;
        hand_req1.send_mess();
        std::cout<<"Handshake request receive Test"<<std::endl;
        Handshake::Request hand_req2("burak","mac2");
        hand_req2.receive_message(hand_req1.test_json());
        hand_req2.send_mess();

        Handshake::Response hand_resp1(200);
        std::cout<<"Handshake response send Test"<<std::endl;
        hand_resp1.send_mess();
        std::cout<<"Handshake response receive Test"<<std::endl;
        Handshake::Response hand_resp2(300);
        hand_resp2.receive_message(hand_resp1.test_json());
        hand_resp2.send_mess();


        std::cout << "HANDSHAKE TEST END" << std::endl<<std::endl;
    /*HANDSHAKE TEST END*/





    /* DIRINFO TEST START */
        std::cout << "DIRINFO TEST START" << std::endl<<std::endl;

        DirInfo::Request dir_req("dir_reqasdasd");
        std::cout << dir_req.get_action() << std::endl << dir_req.get_dir() << std::endl;
        std::cout<<"dir request send Test"<<std::endl;
		dir_req.send_mess();
        std::cout<<"dir request receive Test"<<std::endl;
        DirInfo::Request dir_req2("OOOOO");
        dir_req2.receive_message(dir_req.test_json());
        dir_req2.send_mess();


        DirInfo::Response dir_resp(200);
        dir_resp.add_element(std::string("a"), std::string("b"), std::string("c"), 1, false);
        dir_resp.add_element(std::string("k"), std::string("l"), std::string("m"), 2, true);

        std::vector<DirInfo::Response::DirListElement>& eles = dir_resp.get_elements();
        std::cout << dir_resp.get_statuscode() << " " << dir_resp.get_replyto() << std::endl;
        for(int i=0; i<eles.size(); i++) {
            std::cout << eles[i].change_time << " " << eles[i].entity_id << " " << eles[i].entity_name << " " << eles[i].entity_size << " " << eles[i].type << std::endl;
        }
        
        std::cout<<"dir response send Test"<<std::endl;
        dir_resp.send_mess();
        std::cout<<"dir response receive Test"<<std::endl;
        DirInfo::Response dir_resp2(200);
        dir_resp2.receive_message(dir_resp.test_json());
        dir_resp2.send_mess();

        std::cout << "DIRINFO TEST END" << std::endl << std::endl;
    /* DIRINFO TEST END */


    return 0;
}
