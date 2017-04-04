#include <nar/lib/nlohJson/json.hpp>
#include "messaging_utility.h"
#include <nar/narnode/uservars.h>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
namespace nar {
    namespace Messaging {
        string get_action(string & msg) {
            auto jsn = json::parse(msg);
            return jsn["header"]["action"];
        }

        json transform(string & msg) {
            auto jsn = json::parse(msg);
            return jsn;
        }

        string get_username(string& msg) {
            auto jsn = json::parse(msg);
            return jsn["header"]["username"];
        }

        string get_password(string& msg) {
            auto jsn = json::parse(msg);
            return jsn["header"]["password"];
        }

        string get_current_directory(string& msg) {
            auto jsn = json::parse(msg);
            return jsn["header"]["current_directory"];
        }

        nar::UserVariables get_user_variables(string& msg) {
            auto jsn = json::parse(msg);
            nar::UserVariables uservars(jsn["header"]["current_directory"], jsn["header"]["username"], jsn["header"]["password"]);
            return uservars;
        }
    }
}
