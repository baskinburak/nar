#ifndef NAR_MESSAGING_UTILITY
#define NAR_MESSAGING_UTILITY
#include <nar/lib/nlohJson/json.hpp>
#include "messaging_utility.h"

namespace nar {
    namespace Messaging {
        std::string get_action(std::string & msg);
        nlohmann::json transform(std::string & msg);
    }
}

#endif
