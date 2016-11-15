#ifndef NAR_PUSHFILE_H
#define NAR_PUSHFILE_H

#include <nar/narnode/Task/ITask.h>
#include <string>
namespace nar {
    namespace task {
        class PushFile : public nar::task::ITask {
            private:
                std::string file_path;
            public:
                PushFile(std::string fp): file_path(fp) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}
#endif
