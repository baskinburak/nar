#ifndef NAR_MESSSENDHEADER_H
#define NAR_MESSSENDHEADER_H
#include <string>
#include <map>
#include <vector>
//#include <nar/lib/Socket/Socket.h>
//#include <nar/narnode/utility.h>
#include "/home/fatih/bitirme/nar/nar/lib/nlohJson/json.hpp"

enum SendMessTypes {
                    MINT  = 0,
                    MLINT = 1,
                    MSTRING = 2,
                    MELEMENT = 3,
                    MOBJECT = 4,
                    MOBJECTARR = 5
                };




namespace nar {
	namespace messagetypes{
		typedef struct SendMessElement{
	        std::string name;
	        void * var;
	        SendMessTypes type;
	    } SendMessElement;
	    typedef struct SendMessObject{
	        std::string name;
	        int size;
	        std::vector<SendMessElement> var;
	    } SendMessObject;

        void objSendHandleAlt( nlohmann::json & sent,SendMessObject & obj);

	    void objSendHandle(nlohmann::json & sent,SendMessObject & obj);

	    nlohmann::json eleSendHandle(nlohmann::json & sent,SendMessElement ele);


	    void sendMessage( std::vector<SendMessObject> & narmess);
}

}
#endif
