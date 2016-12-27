#include <string>
//#include <nar/lib/Socket/Socket.h>
#include <vector>
#include <map>
//#include <nar/narnode/utility.h>
#include "/home/utku/nar/nar/lib/nlohJson/json.hpp"


enum MessTypes {
	MINT = 0,
	MLINT = 1,
	MSTRING = 2,
	MOBJ = 3,
	MOBJECTARR = 4
};

namespace nar {

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
	void objRecvHandleArr( nlohmann::json & msg,  std::vector<MessObject> & mArr);

	void eleRecvHandle(MessElement &ele,nlohmann::json &val){
		std::cout << val << " " << ele.name << std::endl;
        switch (ele.type) {
            case MINT: {
                ele.var = (void *)  new int( val[ele.name].get<int>());
                
                break;
			}
            case MLINT: {
                ele.var = (void *)  new long long int(val[ele.name].get<long long int>());
                
                break;
			}
            case MSTRING: {
                ele.var = (void *) new std::string(val[ele.name].get<std::string>());
                
                break;
			}
            case MOBJ: {
                //std::vector<MessElement> temp = *(std::vector<MessElement> *)ele.var;
				//std::cout << "OBJJJJ "<< ele.name << " " << ele.type << std::endl;
				objRecvHandle(  val[ele.name] , *((MessObject *) ele.var)  );
                break;
			}
            case MOBJECTARR: {
                //std::vector<MessObject> temp = *(std::vector<MessObject> *)ele.var;
				//objRecvHandleArr(  val[ele.name] , *((std::vector<MessObject> *) ele.var)  );
                break;
			}
			default: {
				std::cout << "BURDA" << std::endl;
			}
        }
		//std::cout << "ele End" << std::endl;
    }

/*  
            for (nlohmann::json::iterator it = dlist.begin(); it != dlist.end(); ++it) {
                dlistsize++;

            } */


    void objRecvHandle( nlohmann::json & msg, MessObject &mArr){
		
		//std::cout << "objRecvHandle " << msg << std::endl;
        for (int i = 0;i<mArr.var.size();i++) {
			//            nlohmann::json temp = eleSendHandle(obj.var[i]);
			
			eleRecvHandle(mArr.var[i],msg);
			
        } //std::cout << "LOOP FINISHED " << std::endl;
				

    }

    /*void objRecvHandleArr( nlohmann::json & msg,  std::vector<MessObject> & mArr){
        for (int i = 0;i<mArr.size();i++) {
			//            nlohmann::json temp = eleSendHandle(obj.var[i]);
			objRecvHandle(msg[mArr[i].name],mArr[i]);
        }

    }*/

    void recvMessage( /*Socket &sck,*/ std::vector<MessElement> & narmess){

        //nlohmann::json msg = nlohmann::json::parse(get_message(sck));

		nlohmann::json msg = nlohmann::json::parse(std::string(
													"{\"payload\":{\"foo\":31,\"bar\":\"bak hele\"}}"	
													));
		nlohmann::json msg2 = nlohmann::json::parse(std::string(
													"{\"payload\":{\"dir_list\":{\"dir_id\":313131,\"dir_name\":\"cukubik\"}}}"	
													));

		nlohmann::json msg3 = nlohmann::json::parse(std::string(
													"{\"payload\":{\"dir_list\":{  \"dir0\":{\"dir_id\":556655,\"dir_name\":\"mememe\"},\"dir1\":{\"dir_id\":828282,\"dir_name\":\"cukubik\"} }}}"	
													));


        if(narmess.size()<1){
            std::cout<<"Messaging System Error"<<std::endl;
            return;
        }
			
		//nlohmann::json::iterator jit = msg.begin();
		for (int it = 0; it < narmess.size(); ++it) {
        	//objRecvHandle(msg[narmess[it].name], narmess[it]);
			eleRecvHandle(narmess[it],msg3 );
        }

		
		
    }


}

