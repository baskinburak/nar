#include <iostream>
#include <cstdlib>
#include "FileKeeper.h"
using namespace std;
using namespace nar;

int main(int argc,char * argv[]) {

	vector<string> hold;
	string a = "testfiles/somefile0";
	string b = "testfiles/somefile2";
	string c = "testfiles/somefile3";
	string d = "testfiles/somefile4";
	hold.push_back(a);
	hold.push_back(b);
	hold.push_back(c);
		
	char buffer0[512];
	char buffer1[128];
	char buffer2[5];

	FileKeeper holder(c);
	holder.setFileMap(hold);
	cout<<"first print"<<endl;
	holder.printMap();

	hold.clear();
	hold.push_back(c);
	hold.push_back(a);
	hold.push_back(b);

	cout<<"second print"<<endl;
	holder.setFileMap(hold);
	holder.printMap();
	cout<<"third print"<<endl;
	hold.clear();
	holder.setFileMap(hold);
	holder.printMap();
	cout<<"forth print"<<endl;
	holder.insertOneToMap(a);
	holder.printMap();
	cout<<"five print"<<endl;
	holder.insertOneToMap(b);
	holder.printMap();
	cout<<"six print"<<endl;
	holder.insertOneToMap(c);
	holder.printMap();
	
	cout<<holder.getBytes(0,3,buffer2)<<" "<<buffer2<<endl;

	cout<<holder.getBytes(2,3,buffer2)<<" "<<buffer2<<endl;
	//holder.getBytesMap(c,2,2,buffer2);
	//cout<<buffer2<<endl;
	//holder.removeOneFromMap(d);
	
	return 0;

}