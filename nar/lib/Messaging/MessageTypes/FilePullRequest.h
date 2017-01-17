#ifndef NAR_MESSFILEPULLREQUESTREQUEST_H
#define NAR_MESSFILEPULLREQUESTREQUEST_H
#include <string>
#include "RequestHeader.h"
#include "ResponseHeader.h"
#include <vector>

namespace nar {
    namespace messagetypes {
        namespace FilePullRequest {
            class Request : public RequestHeader {
                private:
                    std::string dir;
                    std::string filename;
                public:
                    Request(std::string fn, std::string d):
                            RequestHeader(std::string("file_pull_request")), dir(d), filename(fn) {}
                    std::string& get_filename();
                    std::string& get_dir();
            };

            class Response : public ResponseHeader {
                public:
                    struct PeerListElement {
                        std::string peerid;
                        std::string chunkid;
                        std::string token;
                    };
                    Response(int statcode, std::string fid, unsigned long long int cs): ResponseHeader(statcode, std::string("file_pull_request")), fileid(fid), chunksize(cs) {}
                    Response(int statcode, std::string fid, unsigned long long int cs, std::vector<struct PeerListElement>& eles): ResponseHeader(statcode, std::string("file_pull_request")), fileid(fid), chunksize(cs), elements(eles) {}
                    void add_element(struct PeerListElement& ele);
                    void add_element(std::string pid, std::string cid, std::string token);
                    std::vector<struct PeerListElement>& get_elements();
                    std::string& get_fileid();
                    unsigned long long int get_chunksize();
                private:
                    unsigned long long int chunksize;
                    std::string fileid;
                    std::vector<struct PeerListElement> elements;
            };
        }
    }
}

#endif
