#include "MessageRS.h"

void nar::messagetypes::objSendHandle( nlohmann::json & sent,SendMessObject & obj){
    nlohmann::json another;
    for(int i = 0;i<obj.var.size();i++){
        nar::messagetypes::eleSendHandle(another,obj.var[i]);
        //std::cout<<sent.dump()<<std::endl;

    }
    sent[obj.name] = another;
}
void nar::messagetypes::objSendHandleAlt( nlohmann::json & sent,SendMessObject & obj){
    nlohmann::json another;
    for(int i = 0;i<obj.var.size();i++){
        nar::messagetypes::eleSendHandle(another,obj.var[i]);
        //std::cout<<sent.dump()<<std::endl;

    }
    sent = another;
}

nlohmann::json nar::messagetypes::eleSendHandle(nlohmann::json & sent,SendMessElement ele){
    nlohmann::json newJ;
    switch (ele.type) {
        case MINT: {
            int temp = *(int *)ele.var;
            sent[ele.name] = temp;
            break;
        }
        case MLINT: {
            long long int temp = *(long long int *)ele.var;
            sent[ele.name] = temp;
            break;
        }
        case MSTRING: {
            std::string temp = *(std::string *)ele.var;
            sent[ele.name] = temp;
            break;
        }
        case MELEMENT: {
            std::vector<SendMessElement> temp = *(std::vector<SendMessElement> *)ele.var;
            sent[ele.name]= nlohmann::json::array();
            for(int i=0;i<temp.size();i++){

                 nar::messagetypes::eleSendHandle(newJ,temp[i]);
                 sent[ele.name][i] = newJ;
                 newJ.clear();
            }

            break;
        }
        case MOBJECT: {
            std::vector<SendMessObject> temp = *(std::vector<SendMessObject> *)ele.var;
            for(int i=0;i<temp.size();i++){
                nar::messagetypes::objSendHandle(newJ,temp[i]);
            }
            sent[ele.name] = newJ;
            break;
        }
        case MOBJECTARR: {
            std::vector<SendMessObject> temp = *(std::vector<SendMessObject> *)ele.var;
            nlohmann::json holder= nlohmann::json::array();
            for(int i=0;i<temp.size();i++){
                nar::messagetypes::objSendHandleAlt(newJ,temp[i]);
                holder[i] = newJ;
                newJ.clear();

            }
            sent[ele.name] = holder;
            break;
        }
        default:
            return newJ;
    }

}

void nar::messagetypes::sendMessage( std::vector<SendMessObject> & narmess){
    nlohmann::json sent;
    if(narmess.size()<1){
        std::cout<<"error"<<std::endl;
        return;
    }
    for(int i=0;i<narmess.size();i++){
        objSendHandle(sent,narmess[i]);
    }
    std::cout<<sent.dump()<<std::endl;

}
