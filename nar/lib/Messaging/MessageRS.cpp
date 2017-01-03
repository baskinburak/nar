#include "MessageRS.h"

void makeElement(nar::messagetypes::MessElement &ele, const char *name, MessTypes type) {
	ele.name = std::string(name);
	ele.type = type;
}


void nar::messagetypes::objRecvHandle( nlohmann::json & msg, MessObject &mArr)
{
        for (int i = 0;i<mArr.var.size();i++) {
			//            nlohmann::json temp = eleSendHandle(obj.var[i]);
			
			eleRecvHandle(mArr.var[i],msg);
			
        } //std::cout << "LOOP FINISHED " << std::endl;
}

void nar::messagetypes::eleRecvHandle(MessElement &ele,nlohmann::json &val){
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
            case MARRELEMENT: {
				MessElementArr *arr = ( MessElementArr * ) ele.var;
				nlohmann::json::iterator it = val[ele.name].begin();
				for(int i = 0; i < arr->vars.size(); ++i) {
					std::cout << *it << std::endl;   
					objRecvHandle(*it, *((MessObject *) (arr->vars[i])) );					
					it++;
				} 
                break;
			}
			default: {
				std::cout << "BURDA" << std::endl;
			}
        }
		//std::cout << "ele End" << std::endl;
}

void nar::messagetypes::recvFillMessage( /*Socket &sck,*/ std::vector<MessElement> & narmess) {

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

		nlohmann::json msg4 = nlohmann::json::parse(std::string(
													"{\"payload\":{\"peer_list\":[{\"peer_id\":\"mememe\",\"chunk_id\":565656},{\"peer_id\":\"mamamama\",\"chunk_id\":828282}]}}"	
													));


        if(narmess.size()<1){
            std::cout<<"Messaging System Error"<<std::endl;
            return;
        }
			
		//nlohmann::json::iterator jit = msg.begin();
		for (int it = 0; it < narmess.size(); ++it) {
        	//objRecvHandle(msg[narmess[it].name], narmess[it]);
			eleRecvHandle(narmess[it],msg4 );
        }
		
}

void* nar::messagetypes::recvMessage2(Socket &sck, MessageTypes &ms) {

	return NULL;
}


