#ifndef NAR_ACTION_REGISTER_MACHINE_H
#define NAR_ACTION_REGISTER_MACHINE_H
#include <iostream>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>
#include <nar/narserver/Database.h>
#include <nar/narserver/dbstructs.h>

std::string charlist("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!#$%&'()*+,-./:;<=>?@[]^_`{|}~");
nar::Database db;
std::string generate_machineId() {
    int idLen = 150;
    std::string machineId;
    machineId.resize(idLen);
    nar::Machine tempMac;
    do {
        for(int i=0; i<idLen; i++) {
            machineId[i] = charlist[std::rand()%charlist.size()];
        }
        tempMac = db.getMachine(machineId);
    } while(tempMac.machine_id.compare("-1") != 0);


    return machineId;
}

namespace nar {
    namespace ServerActions {
        /*
         * Machine register functionality for server
         *
         *
         * @author: Fatih
         * @param: inf, nar::SockInfo* , holds receiving socket information
         * @param: macreg_req, MessageTypes::MachineRegister::Request &, holds the received message'c class data
         * @tested: no
         * @todo: need to be sure that sent machine_id received and processed correctly by receiver side
         * @return: void
        */
        void machine_register(nar::SockInfo* inf, MessageTypes::MachineRegister::Request & macreg_req, nar::Database* db) {


                std::string machine_id = generate_machineId();
                MessageTypes::MachineRegister::Response macreg_resp(200,machine_id);
                macreg_resp.send_mess(inf->getSck());
                nar::Machine newMac;
                newMac.machine_id = machine_id;
                db->insertMachine(newMac);
                return;
            }
    }
}

#endif
