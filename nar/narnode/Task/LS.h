#ifndef NAR_DAEMON_TASK_LS_H
#define NAR_DAEMON_TASK_LS_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>

#include <nar/lib/Messaging/MessageTypes/IPCLs.h>
#include <nar/lib/Messaging/MessageTypes/DirInfo.h>

#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>

namespace nar {
    namespace DaemonTask {
        char* masctime(const struct tm *timeptr);
        get_dir_info(nar::MessageTypes::IPCLs::Request ls_req,nar::Global* globals);
    }
}

#endif  //NAR_DAEMON_TASK_LS_H
