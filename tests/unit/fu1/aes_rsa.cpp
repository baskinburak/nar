#include <iostream>
#include <string>
#include <sstream>
#include <nar/lib/Cryption/aes.h>
#include <nar/lib/Cryption/rsa.h>
#include <tests/unit/colormod.h>

typedef bool (*fnc_t)(std::stringstream &); // type for conciseness


void aes_gen_key(std::stringstream & ss) {
    std::string bla;
    AesCryptor::generate_key(bla, 7);
    ss << "Generated Aes Key with length 7: " << bla << std::endl;
    return;
}

bool const_enc_dec(std::stringstream &ss) {
    std::string msg = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

    std::string a("hop1612312345678");
    ss << "AesCryptor instantiated with key: " << a << std::endl;
    ss << "Original String: \n" << msg << std::endl << std::endl<< std::endl;
    AesCryptor cry(a);
    std::string resul;
    cry.encrypt(msg,resul);
    ss << "Encrypted string:\n" << "Len: " << resul.length() <<  "\n str:" <<  resul.substr(0,256) << std::endl;
    std::string bla;
    cry.decrypt(resul,bla);
    ss << "\nDecrypted string: \n" << std::endl;
    ss <<  bla << std::endl;
    aes_gen_key(ss);
	
	if ( bla == msg ) {
		ss << "Strings are identical.\n" ;
		return true;
	}
	else {
		ss << "\nStrings are not identical.\n";
		return false;
	}

}

bool rsa_all(std::stringstream &ss) {
std::string msg = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";

    std::string pub;
    std::string pri;
    RsaCryptor::generate_key_pair(pub,pri);

    ss << "Generated Keys:\nPub: "<< pub << "\nPri: " << pri << std::endl;
   
    RsaCryptor cry;
    cry.set_pub(pub);

    std::string resul;
    ss << "Message to encrypt: \n" << msg << std::endl;
    cry.encrypt(msg,resul);
    ss << "Encrypted Message with length: "<<resul.length() <<  " and message: \n " <<  resul.substr(0,256) << std::endl;
    std::string bla;
    RsaCryptor cry2;
    cry2.set_pri(pri);
    cry2.decrypt(resul,bla);
    ss << std::endl<< std::endl <<"Decrypted Message:\n " <<  bla << std::endl;
    if (bla == msg) {
		ss << "Strings are identical.\n" ;
		return true;
    }
    else {
		ss << "\nStrings are not identical.\n";
		return false;
    }

}



void makeTest(fnc_t test_func, const char* _testName) {
	std::string testName(_testName);
	std::cout << "testing "<< testName << " ..."<< std::endl;
	std::stringstream ss;	
	bool result;

	result = test_func(ss);
	if (result) Color::Modifier::printPass();
	else Color::Modifier::printFail();
	return;	
}

int main() {
    std::stringstream ss;
    bool result;

    makeTest(const_enc_dec, "FU1" );
    makeTest(rsa_all, "FU2");

    return 0;
}


