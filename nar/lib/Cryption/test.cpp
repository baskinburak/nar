#include <iostream>
#include <string>
#include "aes.h"
#include "rsa.h"

void fnc1() {
    
    std::string msg = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

    std::string a("hop1612312345678");

    AesCryptor cry(a);
    std::string resul;
    //cry.encrypt(msg,resul);
    std::cout << resul.length() <<  " " <<  resul.substr(0,256) << std::endl;
    std::string bla;
    //cry.decrypt(resul,bla);
    std::cout << "Deneme 123" << std::endl;
    std::cout << std::endl<< std::endl << bla << std::endl;


}

void fnc2() {
std::string msg = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

    std::string pub;
    std::string pri;
    RsaCryptor::generate_key_pair(pub,pri);   
    RsaCryptor cry;
    cry.set_pub(pub);


    std::string resul;
    cry.encrypt(msg,resul);
    std::cout << resul.length() <<  " " <<  resul.substr(0,256) << std::endl;
    std::string bla;
    RsaCryptor cry2;
    cry2.set_pri(pri);
    cry2.decrypt(resul,bla);
    std::cout << "Deneme 123" << std::endl;
    std::cout << std::endl<< std::endl << bla << std::endl;


}


void fnc3() {
    std::string bla;
    AesCryptor::generate_key(bla, 7);
    std::cout << "Pass: "<< bla << std::endl;
}


int main() {
    fnc2();

    return 0;
}




