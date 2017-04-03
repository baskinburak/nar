#ifndef NAR_DAEMONTASKS_H
#define NAR_DAEMONTASKS_H

namespace nar {
    namespace ActiveTask {
        bool user_authenticate(nar::Socket* skt, nar::UserVariables& uservars);
        
        class IActiveTask {
            private:
                nar::Global* _globals;
                nar::UserVariables* _vars;
            public:
                IActiveTask(globals, vars);
                virtual void run(nar::Socket* ipc_socket, nar::MessageTypes::IPCBaseRequest* req) = 0;
        };

        class LS : public IActiveTask {
            private:
            public:
                LS(nar::Global* globals, nar::UserVariables* variables);
                void run(nar::Socket* ipc_socket, nar::MessageTypes::IPCLs::Request* req);
        };

        class Push : public IActiveTask {
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
