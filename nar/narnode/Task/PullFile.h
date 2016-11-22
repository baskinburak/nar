#ifndef NAR_PULLFILE_H
#define NAR_PULLFILE_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <string>


namespace nar {
    namespace task {
        class PullFile : public nar::task::ITask {
            private:
                std::string file_name;
            public:
                PullFile(std::string fn): file_name(fn) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}
#endif
