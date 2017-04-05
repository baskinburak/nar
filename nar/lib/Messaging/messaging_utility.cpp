#include <nar/lib/nlohJson/json.hpp>
#include "messaging_utility.h"
#include <nar/narnode/uservars.h>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
string nar::Messaging::get_action(string & msg) {
    auto jsn = json::parse(msg);
    return jsn["header"]["action"];
}

json nar::Messaging::transform(string & msg) {
    auto jsn = json::parse(msg);
    return jsn;
}

string nar::Messaging::get_username(string& msg) {
    auto jsn = json::parse(msg);
    return jsn["header"]["username"];
}

string nar::Messaging::get_password(string& msg) {
    auto jsn = json::parse(msg);
    return jsn["header"]["password"];
}

string nar::Messaging::get_current_directory(string& msg) {
    auto jsn = json::parse(msg);
    return jsn["header"]["current_directory"];
}

nar::UserVariables nar::Messaging::get_user_variables(string& msg) {
    auto jsn = json::parse(msg);
    nar::UserVariables uservars(jsn["header"]["current_directory"], jsn["header"]["username"], jsn["header"]["password"]);
    return uservars;
}
