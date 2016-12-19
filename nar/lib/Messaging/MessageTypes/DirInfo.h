#ifndef NAR_MESSDIRINFO_H
#define NAR_MESSDIRINFO_H

#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <string>
#include <vector>
namespace nar {
    namespace messagetypes {
        namespace DirInfo {
            class Request : public RequestHeader {
                private:
                    std::string dir;
                public:
                    Request(std::string d) : RequestHeader(std::string("get_dir_info")), dir(d) {}
                    std::string& get_dir();
            };

            class Response : public ResponseHeader {
                public:
                    struct DirListElement {
                        std::string changetime;
                        std::string entityid;
                        std::string entityname;
                        unsigned long long int entitysize;
                        bool type; // true for DIR, false for FILE
                    };
                    Response(int statcode, std::vector<struct DirListElement>& e) : ResponseHeader(statcode, std::string("get_dir_info")), elements(e) {}
                    Response(int statcode) : ResponseHeader(statcode, std::string("get_dir_info")) {}
                    std::vector<struct DirListElement>& get_elements();
                    void add_element(struct DirListElement& ele);
                    void add_element(std::string ct, std::string ei, std::string en, unsigned long long int s, bool t);
                private:
                    std::vector<struct DirListElement> elements;
            };
        }
    }
}

#endif
