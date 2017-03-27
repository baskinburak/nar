#include "MessageTypes/IPCConfig.h"
#include "MessageTypes/IPCLs.h"
#include "MessageTypes/IPCPull.h"
#include "MessageTypes/IPCPush.h"
#include "MessageTypes/IPCRegister.h"
#include "MessageTypes/IPCStatus.h"

#include <iostream>

using namespace nar::MessageTypes;

int main() {

    /*Test1*/
    std::cout << "IPCConfig request test starts..." << std::endl;
    IPCConfig::Request ipc_req1("server_ip", "127.0.0.0");
    std::cout<<"action_name: "<<ipc_req1.get_action_name()
            <<" var: "<<ipc_req1.get_var()
            <<" value: "<<ipc_req1.get_value()
            <<std::endl;
    std::cout<<"IPCConfig request send test"<<std::endl;
    //ipc_req1.send_action(skt);
    std::cout<<"IPCConfig request receive test"<<std::endl;
    //ipc_req1.print_loop(skt);
    std::string s_tmp = ipc_req1.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp<<std::endl;
    std::cout << "IPCConfig request test ends..." << std::endl<<std::endl;

    std::cout << "IPCConfig response test starts..." << std::endl<<std::endl;
    IPCConfig::Response ipc_resp1(95,200);
    std::cout<<"progress: "<<ipc_resp1.get_progress()
            <<" process_name: "<<ipc_resp1.get_process_name()
            <<" status code: "<<ipc_resp1.get_status_code()<<std::endl;
    std::cout<<"IPCConfig response send test"<<std::endl;
    //ipc_resp1.send_message_progress(skt,100);
    //ipc_resp1.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp1.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCConfig response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;


    /*Test2*/
    std::cout << "IPCLs request test starts..." << std::endl;
    IPCLs::Request ipc_req2("/"); //directory
    std::cout<<"action name: "<<ipc_req2.get_action_name()
            <<" dir: "<<ipc_req2.get_dir_name()<<std::endl;
    std::cout<<"IPCLs request send test"<<std::endl;
    //ipc_req2.send_action(skt);
    std::cout<<"IPCLs request receive test"<<std::endl;
    //ipc_req2.print_loop(skt);
    std::string s_tmp2 = ipc_req2.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp2<<std::endl;
    std::cout << "IPCLs request test ends..." << std::endl;

    std::cout << "IPCLs response test starts..." << std::endl<<std::endl;
    IPCLs::Response ipc_resp2(95,200);
    std::cout<<"progress: "<<ipc_resp2.get_progress()
            <<" process_name: "<<ipc_resp2.get_process_name()
            <<" status code: "<<ipc_resp2.get_status_code()<<std::endl;
    std::cout<<"IPCLs response send test"<<std::endl;
    //ipc_resp2.send_message_progress(skt,100);
    //ipc_resp2.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp2.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCLs response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;

    /*Test3*/
    std::cout << "IPCPull request test starts..." << std::endl;
    IPCPull::Request ipc_req3("test.cpp","/"); //file name and current directory
    std::cout<<"action name: "<<ipc_req3.get_action_name()
            <<" file name: "<<ipc_req3.get_file_name()
            <<" current directory: "<<ipc_req3.get_cur_dir()<<std::endl;
    std::cout<<"IPCPull request send test"<<std::endl;
    //ipc_req3.send_action(skt);
    std::cout<<"IPCPull request receive test"<<std::endl;
    //ipc_req3.print_loop(skt);
    std::string s_tmp3 = ipc_req3.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp3<<std::endl;
    std::cout << "IPCPull request test ends..." << std::endl;

    std::cout << "IPCPull response test starts..." << std::endl<<std::endl;
    IPCPull::Response ipc_resp3(95,200);
    std::cout<<"progress: "<<ipc_resp3.get_progress()
            <<" process_name: "<<ipc_resp3.get_process_name()
            <<" status code: "<<ipc_resp3.get_status_code()<<std::endl;
    std::cout<<"IPCPull response send test"<<std::endl;
    //ipc_resp3.send_message_progress(skt,100);
    //ipc_resp3.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp3.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCPull response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;

    /*Test4*/
    std::cout << "IPCPush request test starts..." << std::endl;
    IPCPush::Request ipc_req4("test.cpp"); //file name
    std::cout<<"action name: "<<ipc_req4.get_action_name()
            <<" file name: "<<ipc_req4.get_file_name()<<std::endl;
    std::cout<<"IPCPush request send test"<<std::endl;
    //ipc_req4.send_action(skt);
    std::cout<<"IPCPush request receive test"<<std::endl;
    //ipc_req4.print_loop(skt);
    std::string s_tmp4 = ipc_req4.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp4<<std::endl;
    std::cout << "IPCPush request test ends..." << std::endl;

    std::cout << "IPCPush response test starts..." << std::endl<<std::endl;
    IPCPush::Response ipc_resp4(95,200);
    std::cout<<"progress: "<<ipc_resp4.get_progress()
            <<" process_name: "<<ipc_resp4.get_process_name()
            <<" status code: "<<ipc_resp4.get_status_code()<<std::endl;
    std::cout<<"IPCPush response send test"<<std::endl;
    //ipc_resp4.send_message_progress(skt,100);
    //ipc_resp4.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp4.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCPush response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;

    /*Test5*/
    std::cout << "IPCRegister request test starts..." << std::endl;
    IPCRegister::Request ipc_req5("dogu"); //user name
    std::cout<<"action name: "<<ipc_req5.get_action_name()
            <<" user name: "<<ipc_req5.get_user_name()<<std::endl;
    std::cout<<"IPCRegister request send test"<<std::endl;
    //ipc_req5.send_action(skt);
    std::cout<<"IPCRegister request receive test"<<std::endl;
    //ipc_req5.print_loop(skt);
    std::string s_tmp5 = ipc_req5.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp5<<std::endl;
    std::cout << "IPCRegister request test ends..." << std::endl;

    std::cout << "IPCRegister response test starts..." << std::endl<<std::endl;
    IPCRegister::Response ipc_resp5(95,200);
    std::cout<<"progress: "<<ipc_resp5.get_progress()
            <<" process_name: "<<ipc_resp5.get_process_name()
            <<" status code: "<<ipc_resp5.get_status_code()<<std::endl;
    std::cout<<"IPCRegister response send test"<<std::endl;
    //ipc_resp5.send_message_progress(skt,100);
    //ipc_resp5.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp5.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCRegister response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;


    /*Test6*/
    std::cout << "IPCStatus request test starts..." << std::endl;
    IPCStatus::Request ipc_req6;
    std::cout<<"action name: "<<ipc_req6.get_action_name()<<std::endl;
    std::cout<<"IPCStatus request send test"<<std::endl;
    //ipc_req6.send_action(skt);
    std::cout<<"IPCStatus request receive test"<<std::endl;
    //ipc_req6.print_loop(skt);
    std::string s_tmp6 = ipc_req6.get_myrequestjson().dump();
    std::cout << "whole json is: "<<s_tmp6<<std::endl;
    std::cout << "IPCStatus request test ends..." << std::endl;

    std::cout << "IPCStatus response test starts..." << std::endl<<std::endl;
    IPCStatus::Response ipc_resp6(95,200);
    std::cout<<"progress: "<<ipc_resp6.get_progress()
            <<" process_name: "<<ipc_resp6.get_process_name()
            <<" status code: "<<ipc_resp6.get_status_code()<<std::endl;
    std::cout<<"IPCStatus response send test"<<std::endl;
    //ipc_resp6.send_message_progress(skt,100);
    //ipc_resp6.send_message_end(skt);
    std::cout<<"whole json is: "<<ipc_resp6.give_myresponsejson().dump()<<std::endl;
    std::cout << "IPCStatus response test ends" << std::endl;
    std::cout << "-----------------------------------"<< std::endl;

    return 0;
}
