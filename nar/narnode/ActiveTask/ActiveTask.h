#ifndef NAR_DAEMONTASKS_H
#define NAR_DAEMONTASKS_H


#include <nar/lib/Socket/Socket.h>
#include <nar/narnode/global.h>
#include <nar/narnode/uservars.h>
#include <nar/narnode/utility.h>

#include <nar/lib/Messaging/MessageTypes/IPCBaseRequest.h>
#include <nar/lib/Messaging/MessageTypes/IPCLs.h>


namespace nar {
    namespace ActiveTask {
        bool user_authenticate(nar::Socket* skt, nar::UserVariables* uservars);

        class IActiveTask {
            private:
                nar::Global* _globals;
                nar::UserVariables* _vars;
            public:
                IActiveTask(nar::Global* globals,nar::UserVariables* vars) : _globals(globals), _vars(vars) {}
                virtual void run(nar::Socket* ipc_socket, nar::MessageTypes::IPCBaseRequest* req) = 0;
        };

        class LS : public IActiveTask {
            private:
            public:
                LS(nar::Global* globals, nar::UserVariables* variables): IActiveTask(globals,variables) {}
                void run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req);
        };

        class Push : public IActiveTask {
            private:
            public:
                Push(nar::Global* globals, nar::UserVariables* variables): IActiveTask(globals,variables) {}
                void run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req);
        };

        class Pull : public IActiveTask{
        };

        class MakeDir : public IActiveTask {
        };

        class Register : public IActiveTask {
        };

    }
}

#endif
