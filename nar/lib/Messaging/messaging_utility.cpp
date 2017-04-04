#include <nar/lib/nlohJson/json.hpp>
#include "messaging_utility.h"
using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
string nar::Messaging::get_action(string & msg) {
    auto jsn = json::parse(msg.c_str());
    return jsn["header"]["action"];
}

json nar::Messaging::transform(string & msg) {
    auto jsn = json::parse(msg.c_str());
    return jsn;
}
