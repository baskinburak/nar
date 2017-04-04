#ifndef NAR_MESSAGING_UTILITY
#define NAR_MESSAGING_UTILITY
#include <nar/lib/nlohJson/json.hpp>
#include <nar/narnode/uservars.h>
#include "messaging_utility.h"

namespace nar {
    namespace Messaging {
        std::string get_action(std::string & msg);
        nlohmann::json transform(std::string & msg);
        std::string get_username(std::string& msg);
        std::string get_password(std::string& msg);
        std::string get_current_directory(std::string& msg);
        nar::UserVariables get_user_variables(std::string& msg);
    }
}

#endif
