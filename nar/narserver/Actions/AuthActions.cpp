#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>
#include <nar/lib/Cryption/aes.h>
#include <nar/narserver/Actions/AuthActions.h>
#include <nar/lib/Messaging/messaging_utility.h>


void nar::AuthAction::authentication_dispatcher(nar::ServerGlobal* s_global, nar::Socket* skt, nar::DBStructs::User& user) {
    std::string message = get_message(skt);
    std::string action = Messaging::get_action(message);
    if(action == std::string("file_push_request")) {
        nar::MessageTypes::FilePush::Request req;
        req.receive_message(message);
        push_file_action(s_global,skt,req,user);
    } else if(action == std::string("file_pull_request")) {

    } else if(action == std::string("machine_register")) {
        nar::MessageTypes::MachineRegister::Request req;
        req.receive_message(message);
        machine_register_action(s_global,skt,req,user);
    }
}

void nar::AuthAction::push_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::FilePush::Request& req, nar::DBStructs::User& user) {
    unsigned long long int file_size = req.get_file_size();
    std::string dir_name = req.get_dir_name();
    std::string file_name = req.get_file_name();
    nar::Database* db = s_global->get_db();
}

void nar::AuthAction::machine_register_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::MachineRegister::Request& req, nar::DBStructs::User& user) {

    long long int machine_quota = req.get_machine_quota();
    long long int machine_diskspace = req.get_machine_diskspace();

    nar::DBStructs::Machine mac;
    mac.machine_quota = machine_quota;
    mac.machine_diskSpace = machine_diskspace;
    mac.user_id = user.user_id;
    nar::Database* db = s_global->get_db();

    std::string mac_id;
    while (1) {
        AesCryptor::generate_key(mac_id,150);
        try {
            mac.machine_id = mac_id;
            db->insertMachine(mac);
            break;
        } catch (...) {
            continue;
        }
    }
    nar::MessageTypes::MachineRegister::Response resp(200,mac_id);
    resp.send_mess(skt);
    return;
}
