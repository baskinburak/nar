#include <nar/narnode/Task/LS.h>
#include <nar/narnode/utility.h>
/*

STATUS CODES
200 -> [SUCC] Request successul.
301 -> [FAIL] User does not exist.
302 -> [FAIL] Directory does not exist.
REQ
{
    "header": {
        "channel":"ps"
        "action":"get_user_dir_info"
    },
    "payload": {
        "user_name": name,
        "dir_name": dir,
    }

}
RES
{
    "header":{
        "channel":"sp",
        "status-code":"2xx" OR "3xx",
        "reply-to":"get_user_dir_info"
    },
    "payload":{
        "file-list": ["1":{"file_id":file_id1, "file_name":file_name1,
                        "file_size":file_size1,"file_type":file_type1},
                            "2":{"file_id":file_id2, "file_name":file_name2,
                        "file_size":file_size2,"file_type":file_type2},
                    ...]
        "dir-list": ["1":{"dir_id":dir_id1, "dir_name":dir_name1,
                        "dir_size":dir_size1,"dir_type":dir_type1},
                            "2":{"dir_id":dir_id2, "dir_name":dir_name2,
                        "dir_size":dir_size2,"dir_type":dir_type2},
                    ...]
    }
}



*/


bool nar::task::LS::sendReqJson(nlohmann::json &js ,nar::Socket* con_socket){
    std::string jreq = js.dump();
    std::string jreq_size = std::to_string(jreq.length());
    jreq = jreq_size + std::string(" ")+ jreq;
    std::cout<<"jreq "<<jreq<<std::endl;

    char * jreq_char  = new char[jreq.length() + 1];
    strcpy(jreq_char, jreq.c_str());
    jreq_char[jreq.length()] = '\0';

    con_socket -> send(jreq_char ,jreq.length() + 1);
    std::cout << "sendReq success" << std::endl;
    delete [] jreq_char;
}


void nar::task::LS::getResJson(nlohmann::json &js,nar::Socket* con_socket){
    std::string response = nar::get_message(*con_socket);
    js = nlohmann::json::parse(response);
}


void nar::task::LS::createReqJson(nlohmann::json &js,std::string & user_name){
    js["header"] = { {"channel", "ps"}, {"action", "get_user_dir_info"} };
    js["payload"] = { {"user_name", user_name}, {"dir_name", _dir} };
}

nar::Socket* nar::task::LS::createServerConnection(nar::Global* globals){
	nar::Socket *serverSck = new nar::Socket();
	serverSck->create();
	serverSck->connect(globals->get_narServerIp(),globals->get_narServerPort());
	return serverSck;
}
void nar::task::LS::run(int unx_sockfd, nar::Global* globals) {
    std::string user_name = globals->get_username();
    nlohmann::json p_to_s;
    nlohmann::json s_to_p;
    createReqJson(p_to_s,user_name);
    std::cout<<"before con socket"<<std::endl;
    nar::Socket *con_socket = createServerConnection(globals);
    std::cout<<"connection success"<<std::endl;
    if (!ITask::handshake(*con_socket, globals)){
        std::cout<<"hand_shake_fail"<<std::endl;
    }
    std::cout<<"handshake success"<<std::endl;
    sendReqJson(p_to_s,con_socket);
    std::cout<<"sendReqJson success"<<std::endl;
    getResJson(s_to_p,con_socket);
    std::cout<<"getResJson success" <<std::endl;
    std::cout<<s_to_p<<std::endl;


}
