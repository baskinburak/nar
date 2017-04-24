#include "IPCDeleteFile.h"

std::string nar::MessageTypes::IPCDeleteFile::Request::get_file_name(){
    return _file_name;
}

std::string nar::MessageTypes::IPCDeleteFile::Request::get_dest_dir() {
    return _dest_dir;
}
void nar::MessageTypes::IPCDeleteFile::Request::set_file_name(std::string dn){
    this->_file_name = dn;
}
void nar::MessageTypes::IPCDeleteFile::Request::set_dest_dir(std::string dd) {
    this->_dest_dir = dd;
}

nlohmann::json nar::MessageTypes::IPCDeleteFile::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "delete_file";
    json_to_sent["payload"]["file_name"] = _file_name;
    json_to_sent["payload"]["dir_name"] = _dest_dir;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCDeleteFile::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["file_name"] = this->_file_name;
    jsn["payload"]["dir_name"] = this->_dest_dir;
    return jsn;
}

void nar::MessageTypes::IPCDeleteFile::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_file_name = jsn["payload"]["file_name"];
    this->_dest_dir = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCDeleteFile::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCDeleteFile::Request::print_loop(nar::Socket* skt) {

    return;
}

/*
char* nar::MessageTypes::IPCDeleteFile::Request::masctime(const struct tm *timeptr)
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


nlohmann::json nar::MessageTypes::IPCDeleteFile::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    return resp_json;
}
