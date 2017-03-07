#include "ServerGlobal.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
    nar::ServerGlobal sg;
    nar::Database* db = sg.get_db();
    cout<<db->getPass()<<" "<<db->getUserNameOfDatabase()<<endl;
    return 0;
}
