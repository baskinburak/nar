#ifndef NAR_DAEMON_TASK_CONFIG_H
#define NAR_DAEMON_TASK_CONFIG_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
namespace nar {
    namespace DaemonTask {
        class Config : public nar::DaemonTask::ITask {

        };
    }
}

#endif
