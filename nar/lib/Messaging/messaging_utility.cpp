#include <nar/lib/nlohJson/json.hpp>
#include "messaging_utility.h"
#include <nar/narnode/uservars.h>
#include <nar/lib/Exception/Exception.h>

using std::cout;
using std::endl;
using std::string;
using namespace nlohmann;
string nar::Messaging::get_action(string & msg) {
    try {
        auto jsn = json::parse(msg);
        std::string tmp = jsn["header"]["action"];
        return tmp;
    } catch( std::invalid_argument& e) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Message Not a Json");
    } catch ( std::domain_error& e) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Message Field missing or has incorrect type");
    } catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Bad message received. Dont know reason");
    }

}

json nar::Messaging::transform(string & msg) {
    try {
        auto jsn = json::parse(msg);
        return jsn;
    }
    catch( std::invalid_argument& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Not a Json");
        }
}

string nar::Messaging::get_username(string& msg) {
    try {
        auto jsn = json::parse(msg);
        std::string tmp = jsn["header"]["username"];
        return tmp;
    }
    catch( std::invalid_argument& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Not a Json");
        }
    catch ( std::domain_error& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Field missing or has incorrect type");
        }

}

string nar::Messaging::get_password(string& msg) {
    try {
        auto jsn = json::parse(msg);
        std::string tmp = jsn["header"]["password"];
        return tmp;
    }
    catch( std::invalid_argument& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Not a Json");
        }
    catch ( std::domain_error& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Field missing or has incorrect type");
        }
}

string nar::Messaging::get_current_directory(string& msg) {

    try {
        auto jsn = json::parse(msg);
        std::string tmp = jsn["header"]["current_directory"];
        return tmp;
    }
    catch( std::invalid_argument& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Not a Json");
        }
    catch ( std::domain_error& e)
        {
            throw nar::Exception::MessageTypes::BadMessageReceive("Message Field missing or has incorrect type");
        }
}

nar::UserVariables nar::Messaging::get_user_variables(string& msg) {
    try {
        auto jsn = json::parse(msg);
        std::string cd = jsn["header"]["current_directory"];
        std::string uname = jsn["header"]["username"];
        std::string ps = jsn["header"]["password"];
        nar::UserVariables uservars(cd, uname, ps);
        return uservars;
    }
    catch(...) {
        throw nar::Exception::MessageTypes::BadMessageReceive("Message does not contain user variables");
    }
}
