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
        "action":"dir_info"
    },
    "payload": {
        "user_name": name,
        "dir_name": dir,
    }

}
RES

{
    "header":
        {
            "channel":"sp",
            "reply-to":"dir_info"
        },
        "payload":{
            "dir_list":{
                        "dir0":{
                            "dir_change_time":dir_change_time0,
                            "dir_id":dir_id,
                            "dir_name":dir_name0,
                            "dir_size":dir_size0
                        },...
                        },
            "file_list":{
                        "file0":{
                            "file_change_time":file_change_time0,
                            "file_id":file_id0,
                            "file_name":file_name0,
                            "file_size":file_size0
                        },
                        "file1":{
                            "file_change_time":file_change_time1,
                            "file_id":file_id1,
                            "file_name":file_name1,
                            "file_size":file_size1
                        },...
            }
        }
}




*/

char* nar::task::LS::masctime(const struct tm *timeptr)
{
  static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s%2d %.2d:%.2d",
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday, timeptr->tm_hour,
    timeptr->tm_min);
  return result;
}
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
    js["header"] = { {"channel", "ps"}, {"action", "dir_info"} };
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
    //std::cout<<s_to_p<<std::endl;

    nlohmann::json flist = s_to_p["payload"]["file_list"];
    nlohmann::json dlist = s_to_p["payload"]["dir_list"];
    int flistsize = 0;
    int dlistsize = 0;
    std::string file_name;
    std::string file_size;
    std::string file_change_time;
    for (nlohmann::json::iterator it = flist.begin(); it != flist.end(); ++it) {
        flistsize++;
        /*
        if(it.key().compare(std::string("file_name"))){
            file_name= it.value();
        }else if(it.key().compare(std::string("file_size"))){
            file_name= it.value();
        }else if(it.key().compare(std::string("file_change_time"))){
            file_name= it.value();
        }
        //std::cout << it.key().get<std::string>() << " : " << it.value() << "\n";
        std::cout << file_name<<" "<<file_size<<" "<<file_change_time<<" "<<"file"<<std::endl;*/
    }
    for (nlohmann::json::iterator it = dlist.begin(); it != dlist.end(); ++it) {
        dlistsize++;

    }

    for(int i = 0;i<flistsize;i++){
        std::string ftraverse = std::string("file")+std::to_string(i);
        nlohmann::json test = s_to_p["payload"]["file_list"][ftraverse];
        std::string send("--->");
        send = send + test["file_name"].get<std::string>() + std::string(" ");
        send = send + std::to_string(test["file_size"].get<time_t>()) + std::string(" ");
        time_t holder = test["file_change_time"].get<time_t>();
        struct tm * timeinfo;
        timeinfo = localtime(&holder);
        send = send + std::string(masctime(timeinfo)) + std::string(" ");
        send = send + std::string("file");
        send_ipc_message(unx_sockfd, send);
    //    delete tm;
    }
    for(int i = 0;i<dlistsize;i++){
        std::string dtraverse = std::string("dir")+std::to_string(i);
        nlohmann::json test = s_to_p["payload"]["dir_list"][dtraverse];
        std::string send("--->");
        send = send + test["dir_name"].get<std::string>() + std::string(" ");
        send = send + std::to_string(test["dir_size"].get<time_t>())  + std::string(" ");
        time_t holder = test["dir_change_time"].get<time_t>();
        struct tm * timeinfo;
        timeinfo = localtime(&holder);
        send = send + std::string(masctime(timeinfo)) + std::string(" ");
        send = send + std::string("dir");
        send_ipc_message(unx_sockfd, send);
    }
    send_ipc_message(unx_sockfd, std::string("END"));

}
