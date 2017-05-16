#include <iostream>
#include <string>
#include <sstream>

#include <nar/narnode/File/File.h>


void constructor_test()
{
	const char* filename = "goodfile";
	const char* mode = "r";
	bool is_temp = false;
	try{
		std::cout << "Testing file constructor of first type" << std::endl;
		nar::File mafile(filename, mode, is_temp);
	}

	catch(nar::Exception::File::UnrecognizedFileMode e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}

	catch(nar::Exception::File::OpenFail e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}
	std::cout << "Succesfully constructed file is ready" << std::endl;
	std::endl << std:endl;
	std::cout << "TEST PASS" << std::endl;

	return;

}

void constructor_test2()
{
	std::string fname = "garfield";
	const char* mode = "r";
	bool is_temp = false;
	try{
		std::cout << "Testing file constructor of first type" << std::endl;
		nar::File mafile(fname, mode, is_temp);
	}

	catch(nar::Exception::File::UnrecognizedFileMode e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}

	catch(nar::Exception::File::OpenFail e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}
	std::cout << "Succesfully constructed file is ready" << std::endl;
	std::endl << std:endl;
	std::cout << "TEST PASS" << std::endl;

	return;
}

void read_test()
{
	char buffer[5000];
	nar::File f("aes.cpp", "r");
	try{
		f.read(buffer, 0, 1024);
	}
	catch(nar::Exception::File::OffsetOutOfBounds e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}

	catch(nar::Exception::DomainError::Negative e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}

	catch(nar::Exception::File::ReadError e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}

	catch(nar::Exception::File::NotOpen e){
		std::cout << e.what() << std::endl;
		std::cout << "TEST FAIL" << std::endl;
		return;
	}
	
	std::cout << "Succesfull in read" << std::endl;
	std::endl << std:endl;
	std::cout << "TEST PASS" << std::endl;

	return;
}

void write_test()
{

}

void write_test2()
{

}

void size_test()
{

}

void close_test()
{

}

void compress_test()
{

}

void decompress_test()
{

}

void encrypt_test()
{

}

void decrypt_test()
{

}



int main()
{
	std::cout << "File test starts..." << std::endl;

	constructor_test();
	constructor_test2();
	read_test();
	write_test();
	write_test2();
	size_test();
	close_test();
	compress_test();
	decompress_test();
	encrypt_test();
	decrypt_test();

	std::cout << "File test ended...you like the results?" << std::endl;
	return 0;
}