#include "IPCMkdir.h"

std::string nar::MessageTypes::IPCMkdir::Request::get_dir_name(){
    return _dir_name;
}

std::string nar::MessageTypes::IPCMkdir::Request::get_dest_dir() {
    return _dest_dir;
}
void nar::MessageTypes::IPCMkdir::Request::set_dir_name(std::string dn){
    this->_dir_name = dn;
}
void nar::MessageTypes::IPCMkdir::Request::set_dest_dir(std::string dd) {
    this->_dest_dir = dd;
}

nlohmann::json nar::MessageTypes::IPCMkdir::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "ls";
    json_to_sent["payload"]["dir_name"] = _dir_name;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCMkdir::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["dir_name"] = this->_dir_name;
    return jsn;
}

void nar::MessageTypes::IPCMkdir::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_dir_name = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCMkdir::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCMkdir::Request::print_loop(nar::Socket* skt) {
	std::vector<std::string> v_items;
	std::string ename=std::string(""), esize=std::string(""), ctime=std::string(""), type=std::string("");
    while(true){
		std::string items = std::string("");
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(tmp);
        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
		else if(received["header"]["reply_to"] == std::string("mkdir")) {

			items += (std::string(">>>") + std::string(" ")  + std::string(" success ") +  std::string(" ") +std::string("\n")) ;
			v_items.push_back(items);
		}
    }
	for(int i=0;i<v_items.size();i++) {
		std::cout << v_items[i];
	}
    return;
}

/*
char* nar::MessageTypes::IPCMkdir::Request::masctime(const struct tm *timeptr)
{
	static const char wday_name[][4] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[][4] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char result[26];
	sprintf(result, "%.3s%2d %.2d:%.2d", mon_name[timeptr->tm_mon], timeptr->tm_mday, timeptr->tm_hour, timeptr->tm_min);
	return result;
}
*/


nlohmann::json nar::MessageTypes::IPCMkdir::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    return resp_json;
}
