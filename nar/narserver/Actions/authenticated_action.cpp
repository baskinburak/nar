#include "ServerActions.h"
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>

void nar::ServerAction::authenticated_action(nar::ServerGlobal* s_global, nar::MessageTypes::UserAuthenticationInit::Request& req) {
    std::string username = req.get_username();
    nar::Database* db = s_global->get_db();
    

}
