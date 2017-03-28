#include <nar/narnode/Task/LS.h>
#include <nar/narnode/utility.h>

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

void nar::task::LS::run(nar::Socket* skt, nar::Global* globals) {

    std::string user_name = globals->get_username();
    nar::MessageType::DirInfo::Request dir_req(_dir);
    nar::MessageTypes::DirInfo::Response dir_resp(999);
    //nar::Socket *con_socket = createServerConnection(globals);
    if (!ITask::handshake(skt, globals)){
        std::cout<<"hand_shake_fail"<<std::endl;
    }
    dir_req.send_mess(skt,dir_resp);
    std::vector<struct nar::MessageTypes::DirInfo::Response::DirListElement> items = dir_resp.get_elements();

    for(int i =0;i<items.size();i++) {
        std::string entry(">>>>> ");
        std::string entity_name = items[i].entity_name + std::string(" ");
        std::string entity_id = std::to_string(items[i].entity_id) + std::string(" ");
        std::string entity_size = std::to_string(items[i].entity_size) + std::string(" ");
        std::string change_time = items[i].change_time + std::string(" ");
        time_t holder = std::stoll(change_time);
        struct tm * timeinfo;
        timeinfo = localtime(&holder);
        change_time = std::string(masctime(timeinfo)) + std::string(" ");
        std::string type;
        if(items[i].bool == false) {
            type = std::string(" file");
        } else if(items[i].bool == true) {
            type = std::string(" directory");
        }
        entry += entity_name+entity_id + entity_size + change_time +type;

    }
    send_ipc_message(unx_sockfd, std::string("END"));

}
