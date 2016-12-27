#include <string>
#include <map>
#include <vector>
//#include <nar/lib/Socket/Socket.h>
//#include <nar/narnode/utility.h>
#include "/home/fatih/bitirme/nar/nar/lib/nlohJson/json.hpp"

enum MessTypes {
                    MINT  = 0,
                    MLINT = 1,
                    MSTRING = 2,
                    MELEMENT = 3,
                    MOBJECT = 4
                };




namespace nar {
    typedef struct MessElement{
        std::string name;
        void * var;
        MessTypes type;
    } MessElement;
    typedef struct MessObject{
        std::string name;
        int size;
        std::vector<MessElement> var;
    } MessObject;

    void objSendHandle(nlohmann::json & sent,MessObject & obj);
    nlohmann::json eleSendHandle(nlohmann::json & sent,MessElement ele){
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
                std::vector<MessElement> temp = *(std::vector<MessElement> *)ele.var;
                sent[ele.name]= nlohmann::json::array();
                for(int i=0;i<temp.size();i++){

                     eleSendHandle(newJ,temp[i]);
                     sent[ele.name][i] = newJ;
                     newJ.clear();
                }

                break;
            }
            case MOBJECT: {
                std::vector<MessObject> temp = *(std::vector<MessObject> *)ele.var;
                for(int i=0;i<temp.size();i++){
                    objSendHandle(newJ,temp[i]);
                }
                sent[ele.name] = newJ;
                break;
            }
            default:
                return newJ;
        }

    }

    void objSendHandle( nlohmann::json & sent,MessObject & obj){
        nlohmann::json another;
        for(int i = 0;i<obj.var.size();i++){
            eleSendHandle(another,obj.var[i]);
            //std::cout<<sent.dump()<<std::endl;

        }
        sent[obj.name] = another;
    }
    void sendMessage( std::vector<MessObject> & narmess){
        nlohmann::json sent;
        if(narmess.size()<1){
            std::cout<<"error"<<std::endl;
            return;
        }
        for(int i=0;i<narmess.size();i++){
            objSendHandle(sent,narmess[i]);
            std::cout<<sent.dump()<<std::endl;
        }

    }
}
