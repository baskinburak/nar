#ifndef NAR_MESSAGERS
#define NAR_MESSAGERS

#include <string>
#include "/home/utku/nar/nar/lib/Socket/Socket.h"
#include <vector>
#include <map>
//#include <nar/narnode/utility.h>
#include "/home/utku/nar/nar/lib/nlohJson/json.hpp"

enum MessageTypes {
	ChunkReceiveRequest,
	DirInfo,
	FilePushRequest,
	Handshake,
	InfoChunkPull,
	InfoChunkPush,
	KeepAlive,
	PeerPortPushRequest,
	Register,
};


enum MessTypes {
	MINT = 0,
	MLINT = 1,
	MSTRING = 2,
	MOBJ = 3,
	MARRELEMENT = 4
};



namespace nar {

	namespace messagetypes {

		typedef struct MessElementArr{
			std::vector<void *> vars;
			std::vector< ::MessTypes > types;
		} MessElementArr;


		typedef struct MessElement{
			std::string name;
			void * var;
			::MessTypes type;
		} MessElement;

		typedef struct MessObject{
			int size;
			std::vector<MessElement> var;
		} MessObject;

		void objRecvHandle( nlohmann::json & msg, MessObject &mArr);

		void eleRecvHandle(MessElement &ele,nlohmann::json &val);

		void objRecvHandle( nlohmann::json & msg, MessObject &mArr);

		void recvFillMessage( /*Socket &sck,*/ std::vector<MessElement> & narmess);

		void * recvMessage2(Socket &sck, MessageTypes &ms);
	}
}

void makeElement(nar::messagetypes::MessElement &ele, const char *name, MessTypes type);



#endif 
