#include <iostream>
#include <string>
#include "aes.h"
#include "rsa.h"

void fnc1() {
    
    std::string msg = "Hallo LANA IMA UTKU and you know i love stuff and shit. Get shwifty at your finest hour and enjoy your life while sucking a cock maybe a brendi at one hand and another on the other and stuff but you better .. i mean better get your SCHEIT together man because it is kind of sad to see you drinking water from a bar toilet and doing all those bad cocain stuff and shit other than that i send my regards to your beautiful mother who has been a solid moisy inspiration source for me when i first applied to the school of jigolo thx btw for that beer see you sucker on the other side you know.";

    std::string a("yarak88benaltili");

    AesCryptor cry(a);
    std::string resul;
    //cry.encrypt(msg,resul);
    std::cout << resul.length() <<  " " <<  resul.substr(0,256) << std::endl;
    std::string bla;
    //cry.decrypt(resul,bla);
    std::cout << "DESIFRELENMIS ANAHABER BULTENI" << std::endl;
    std::cout << std::endl<< std::endl << bla << std::endl;


}

void fnc2() {
std::string msg = "Hallo LANA IMA UTKU and you know i love stuff and shit. Get shwifty at your finest hour and enjoy your life while sucking a cock maybe a brendi at one hand and another on the other and stuff but you better .. i mean better get your SCHEIT together man because it is kind of sad to see you drinking water from a bar toilet and doing all those bad cocain stuff and shit other than that i send my regards to your beautiful mother who has been a solid moisy inspiration source for me when i first applied to the school of jigolo thx btw for that beer see you sucker on the other side you know.X";

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
    std::cout << "DESIFRELENMIS ANAHABER BULTENI" << std::endl;
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




