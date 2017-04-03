#include <nar/lib/nlohJson/json.hpp>
using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
namespace nar {
    string find_message_action(string & msg) {
        auto jsn = json::parse(msg.c_str());
        return jsn[header][action];
    }
    json transform(string & msg) {
        auto jsn = json::parse(msg.c_str());
        return jsn;
    }
}
