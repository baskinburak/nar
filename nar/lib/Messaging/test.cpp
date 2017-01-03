/*#include "MessageTypes/DirInfo.h"
#include "MessageTypes/FilePushRequest.h"
#include "MessageTypes/Register.h"
#include "MessageTypes/Handshake.h"
#include "MessageTypes/KeepAlive.h"
#include "MessageTypes/PeerPortPushRequest.h"
#include "MessageTypes/InfoChunkPush.h"
#include "MessageTypes/InfoChunkPull.h"
#include "MessageTypes/ChunkReceiveRequest.h"*/
#include "MessageRS.h"
#include <iostream>

using namespace nar::messagetypes;

void recvTest0() {

	std::vector<MessElement> io;
	MessElement payloadE;
	MessObject payload;
	payload.size = 2;

	MessElement foo;
	MessElement bar;
	foo.name = std::string("foo");
	bar.name = std::string("bar");
	foo.type = (MessTypes) 0;
	bar.type = (MessTypes) 2;
	//foo.var =  (void *)(new int(31) );
	//bar.var =  (void *)(new std::string("bak hele") );
	payload.var.push_back(foo);
	payload.var.push_back(bar);

	payloadE.name = std::string("payload");
	payloadE.type = (MessTypes)3;
	payloadE.var = (void *)(&payload);
	io.push_back(payloadE);
	
	recvMessage(  io );
	
	std::cout << "hobaaaaa" << std::endl;
	std::cout <<  ( (MessObject *)(io[0].var) )->var[0].name << " val " <<  *(( int *) ( ( (MessObject *)(io[0].var) )->var[0].var ))  << std::endl;
	std::cout <<  ( (MessObject *)(io[0].var) )->var[1].name << " val " <<  *(( std::string *) ( ( (MessObject *)(io[0].var) )->var[1].var ))  << std::endl;

}
void recvTest1() {
	std::vector<MessElement> ev;
	MessElement payloadE2;
	MessObject payload2;
	payload2.size = 1;

	MessElement dirL;
	//MessElement fileL;
	dirL.name = std::string("dir_list");
	//fileL.name = std::string("file_list:");
	dirL.type = (MessTypes) 3;
	//fileL.type = (MessTypes) 3;
	
	//foo.var =  (void *)(new int(31) );
	//bar.var =  (void *)(new std::string("bak hele") );
	MessObject dirLO;
	dirLO.size = 2;
	MessElement dId;
	dId.type = (MessTypes)1;
	dId.name = std::string("dir_id");
	MessElement dirName;
	dirName.type = (MessTypes)2;
	dirName.name = std::string("dir_name");
	dirLO.var.push_back(dId);
	dirLO.var.push_back(dirName);
	dirL.var = (void *) (&dirLO);

	payload2.var.push_back(dirL);
	//payload.var.push_back(fileL);


	payloadE2.name = std::string("payload");
	payloadE2.type = (MessTypes)3;
	payloadE2.var = (void *)(&payload2);

	ev.push_back(payloadE2);

	recvMessage(  ev );

	std::cout << *( (std::string *) ( ( (MessObject *) ( ( (MessObject *)(ev[0].var) )->var[0].var))->var[1].var ))  << " val " <<  std::endl;//*(( int *) ( ( (MessObject *)(io[0].var) )->var[0].var ))  << std::endl;
}
void recvTest2() {
	std::vector<MessElement> ev;
	MessElement payloadE2;
	MessObject payload2;
	payload2.size = 1;

	MessElement dirL;
	//MessElement fileL;
	dirL.name = std::string("dir_list");
	//fileL.name = std::string("file_list:");
	dirL.type = (MessTypes) 3;
	//fileL.type = (MessTypes) 3;
	
	//foo.var =  (void *)(new int(31) );
	//bar.var =  (void *)(new std::string("bak hele") );
	MessObject dirLO;
	dirLO.size = 2;

		MessElement dir0E;
		dir0E.name = std::string("dir0");
		dir0E.type = (MessTypes) 3;
		MessObject dir0O;
		dir0O.size = 2;
	
		MessElement dId;
		dId.type = (MessTypes)1;
		dId.name = std::string("dir_id");
		MessElement dirName;
		dirName.type = (MessTypes)2;
		dirName.name = std::string("dir_name");
		dir0O.var.push_back(dId);
		dir0O.var.push_back(dirName);
		dir0E.var = (void *) (&dir0O);

	dirLO.var.push_back(dir0E);


		MessElement dir0E2;
		dir0E2.name = std::string("dir1");
		dir0E2.type = (MessTypes) 3;
		MessObject dir0O2;
		dir0O2.size = 2;

		MessElement dId2;
		dId2.type = (MessTypes)1;
		dId2.name = std::string("dir_id");
		MessElement dirName2;
		dirName2.type = (MessTypes)2;
		dirName2.name = std::string("dir_name");
		dir0O2.var.push_back(dId2);
		dir0O2.var.push_back(dirName2);
		dir0E2.var = (void *) (&dir0O2);
	
	dirLO.var.push_back(dir0E2);

	dirL.var = (void *)(&dirLO);

	payload2.var.push_back(dirL);
	//payload.var.push_back(fileL);


	payloadE2.name = std::string("payload");
	payloadE2.type = (MessTypes)3;
	payloadE2.var = (void *)(&payload2);

	ev.push_back(payloadE2);

	recvMessage(  ev );

	std::cout  << "HELLOOOOO HOW ARE YOU" << std::endl;
	//std::cout << *((long long int *) dId2.var) << std::endl;
	std::cout << *( (long long int *) ((MessObject *) ((MessObject *) ( ( (MessObject *)(ev[0].var) )->var[0].var) ) ->var[0].var )->var[0].var)   << " val " <<  std::endl;//*(( int *) ( ( (MessObject *)(io[0].var) )->var[0].var ))  << std::endl;

	
}

void recvTest3() {
	
	std::vector<MessElement> ev;
	MessElement payloadE2;
	MessObject payload2;
	payload2.size = 1;

	MessElement dirL;
	dirL.name = std::string("peer_list");
	dirL.type = (MessTypes) 4;
	MessElementArr dirLO;

		MessObject dir0O;
		dir0O.size = 2;
	
		MessElement dId;
		dId.type = (MessTypes)2;
		dId.name = std::string("peer_id");
		MessElement dirName;
		dirName.type = (MessTypes)1;
		dirName.name = std::string("chunk_id");
		dir0O.var.push_back(dId);
		dir0O.var.push_back(dirName);

	//dirLO.var.push_back(dir0E);
	dirLO.vars.push_back( (void *) (&dir0O) );

		MessObject dir0O2;
		dir0O2.size = 2;

		MessElement dId2;
		dId2.type = (MessTypes)2;
		dId2.name = std::string("peer_id");
		MessElement dirName2;
		dirName2.type = (MessTypes)1;
		dirName2.name = std::string("chunk_id");
		dir0O2.var.push_back(dId2);
		dir0O2.var.push_back(dirName2);
	
	//dirLO.var.push_back(dir0E2);
	dirLO.vars.push_back( (void *) (&dir0O2) );

	dirL.var = (void *)(&dirLO);

	payload2.var.push_back(dirL);
	//payload.var.push_back(fileL);


	payloadE2.name = std::string("payload");
	payloadE2.type = (MessTypes)3;
	payloadE2.var = (void *)(&payload2);

	ev.push_back(payloadE2);

	recvMessage(  ev );

std::cout << *((long long int *) ((MessObject *)  ((MessElementArr *)  ( (MessElement ) ( (MessObject *)(ev[0].var) )->var[0]).var)->vars[1] )->var[1].var)     << " val " <<  std::endl;

//std::cout << *( (std::string *)((MessObject *) ((MessElement *) ((MessElementArr *) ( ( (MessObject *)(ev[0].var) )->var[0].var) ) ->vars[0])->var )->var[0].var)   << " val " <<  std::endl;//*(( int *)

}
				

int main() {

	recvTest3();

    return 0;
}
