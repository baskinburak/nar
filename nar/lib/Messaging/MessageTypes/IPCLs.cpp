#include "IPCLs.h"

std::string nar::MessageTypes::IPCLs::Request::get_dir_name(){
    return _dir_name;
}

void nar::MessageTypes::IPCLs::Request::set_dir_name(std::string dn){
    this->_dir_name = dn;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "ls";
    json_to_sent["payload"]["dir_name"] = _dir_name;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["dir_name"] = this->_dir_name;
    return jsn;
}

void nar::MessageTypes::IPCLs::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_dir_name = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCLs::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    nar::send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCLs::Request::print_loop(nar::Socket* skt) {
	std::vector<std::string> v_items;
	std::string ename=std::string(""), esize=std::string(""), type=std::string("");
    while(true){
		std::string items = std::string("");
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(tmp);


        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
        int stat = received["payload"]["status_code"];
        if (stat != 200) {
            if(stat/100 == 3) {
                std::cout<<"Your request was not complete or was wrong --- stat"<< stat<<std::endl;
                break;
            } else  if(stat/100 == 4) {
                std::cout<<"Database problem --- stat"<< stat<<std::endl;
                break;
            } else  if(stat/100 == 5) {

                std::cout<<"Some things went wrong in server --- stat"<< stat<<std::endl;
                break;
            } else  if(stat/100 == 6) {
                std::cout<<"Some things went wrong in daemon --- stat"<< stat<<std::endl;
                break;
            }
        }
		else if(received["header"]["reply_to"] == std::string("ls")) {
			ename =	received["payload"]["entity_name"];
			esize = received["payload"]["entity_size"];
			type = received["payload"]["type"];

            std::string change_time = received["payload"]["change_time"];
            time_t holder = std::stoll(change_time);
            struct tm * timeinfo;
            timeinfo = localtime(&holder);
            change_time = std::string(masctime(timeinfo)) + std::string(" ");

			items += (std::string("> ")+ename + std::string(" ") + esize + std::string(" ") + change_time + std::string(" ") + type + std::string("\n")) ;
			v_items.push_back(items);
		}
    }
    if(v_items.size() == 0){
        std::cout << "-> Your nar system is currently empty <-"
    } else {
        for(int i=0;i<v_items.size();i++) {
            std::cout << v_items[i];
        }
    }
    return;
}


char* nar::MessageTypes::IPCLs::Request::masctime(const struct tm *timeptr)
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


std::string nar::MessageTypes::IPCLs::Response::get_entity_name() {
	return this -> _entity_name;
}
std::string nar::MessageTypes::IPCLs::Response::get_entity_size() {
	return this -> _entity_size;
}
std::string nar::MessageTypes::IPCLs::Response::get_change_time() {
	return this -> _change_time;
}
std::string nar::MessageTypes::IPCLs::Response::get_type() {
	return this -> _type;
}

void nar::MessageTypes::IPCLs::Response::set_entity_name(std::string en) {
	this -> _entity_name = en;
}
void nar::MessageTypes::IPCLs::Response::set_entity_size(std::string es) {
	this -> _entity_size = es;
}
void nar::MessageTypes::IPCLs::Response::set_change_time(std::string ct) {
	this -> _change_time = ct;
}
void nar::MessageTypes::IPCLs::Response::set_type(std::string t) {
	this -> _type = t;
}
void nar::MessageTypes::IPCLs::Response::send_message_progress(nar::Socket* skt, int p) {
    nlohmann::json json_to_sent;

    json_to_sent["header"]["reply_to"] = get_reply_to();
    json_to_sent["payload"]["progress"] = get_progress();
    json_to_sent["payload"]["status_code"] = get_status_code();
    int stat = get_status_code();
    if(stat != 200) {
        nar::send_message(skt, json_to_sent.dump());
        return;
    }

    json_to_sent["payload"]["entity_name"] = get_entity_name();
    json_to_sent["payload"]["entity_size"] = get_entity_size();
    json_to_sent["payload"]["change_time"] = get_change_time();
    json_to_sent["payload"]["type"] = get_type();
    nar::send_message(skt, json_to_sent.dump());
    return;
}

nlohmann::json nar::MessageTypes::IPCLs::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
	resp_json["payload"]["entity_name"] = get_entity_name();
	resp_json["payload"]["entity_size"] = get_entity_size();
	resp_json["payload"]["change_time"] = get_change_time();
	resp_json["payload"]["type"] = get_type();
    return resp_json;
}
