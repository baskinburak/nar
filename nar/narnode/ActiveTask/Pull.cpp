#include "ActiveTask.h"
#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/FilePull.h>
#include <nar/lib/Socket/USocket.h>
#include <nar/lib/Messaging/MessageTypes/InfoChunkPull.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>


#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

using std::string;
using std::cout;
using std::endl;


// TO_DO: Last part should handled for file::encrypt,decompress etc..

void nar::ActiveTask::Pull::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCPull::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    std::string file_aes;
    try {
        file_aes = nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError& exp) {
        NAR_LOG<<exp.what()<<std::endl;
        return;
    }
    string file_name,system_dir,nar_dir_name;
    try {
        file_name = req->get_file_name();
        system_dir = req->get_dir_path(); // to be changed
        nar_dir_name =  req->get_current_directory();
    }
    catch (...) {
        NAR_LOG << "Malformed Json, exiting pull" << std::endl;
        return;
    }

    nar::MessageTypes::FilePull::Request pull_req(file_name, nar_dir_name);
    nar::MessageTypes::FilePull::Response pull_resp;
    try {
        pull_req.send_mess(server_sck, pull_resp);
    }
    catch (nar::Exception::ExcpBase& e) {
        NAR_LOG << std::string( "nar_daemon::activepull " ).append(e.what()) << std::endl;
        return;
    }

    if ( pull_resp.get_status_code() == 666 ) {     // Not enough online peer
        nar::MessageTypes::IPCPull::Response ipcpull_resp(3,666);           // Params ?
        ipcpull_resp.send_message_end(ipc_socket);
        return;
    }

    unsigned short rand_port = pull_resp.get_rendezvous_port();
    std::vector<struct nar::MessageTypes::FilePull::Response::PeerListElement> elements = pull_resp.get_elements();
    std::sort(elements.begin(), elements.end());


    boost::filesystem::path tpath(system_dir);
    tpath /= file_name;



    boost::filesystem::path temp;
    try {
        temp = boost::filesystem::unique_path();
    } catch(std::ios_base::failure& Exp) {
        throw nar::Exception::Unknown(Exp.what());
    }
    std::string temp_native = temp.native();

    nar::File* tempfile1 = new nar::File(temp_native,"w",false);
    int i=0;
    try {
        unsigned long offset = 0;
        while( i<elements.size() ) {

            unsigned long stream_id = elements[i].stream_id;
            nar::USocket* cli_sck = new nar::USocket(this->_globals->get_ioserv(), this->_globals->get_server_ip(), rand_port, stream_id);
            char* buf = NULL;
            try {
                cli_sck->connect();
                long int total_read = 0;
                buf = new char [elements[i].chunk_size];
                while(total_read < elements[i].chunk_size) {
                    int len = cli_sck->recv(buf+total_read, elements[i].chunk_size - total_read);
                    total_read += len;
                }
                byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE];
            	CryptoPP::Weak::MD5 hash;
            	hash.CalculateDigest( digest, (const byte*)buf, elements[i].chunk_size );
            	CryptoPP::HexEncoder encoder;
            	std::string output;
            	encoder.Attach( new CryptoPP::StringSink( output ) );
            	encoder.Put( digest, sizeof(digest) );
            	encoder.MessageEnd();

               /* if (  output.compare(elements[i].hashed) ) // Wrong Hash
                {
                    std::cout <<"Benim Hesabim: " << output << std::endl;
                    std::cout <<"Carsidaki Hesap: " << elements[i].hashed << std::endl;
                    throw nar::Exception::Unknown("Wrong Hash");
                } */

                tempfile1->write(buf,total_read);
                offset+=total_read;
            }
            catch (nar::Exception::File::WrongMode& e) {
                NAR_LOG << e.what() << std::endl;
            }
            catch (nar::Exception::File::OffsetOutOfBounds& e) {
                NAR_LOG << e.what() << std::endl;
            }
            catch ( nar::Exception::DomainError::Negative& e) {
                NAR_LOG << e.what() << std::endl;
            }
            catch ( nar::Exception::File::WriteError& e) {
                NAR_LOG << e.what() << std::endl;
            }
            catch ( nar::Exception::File::NotOpen& e) {
                NAR_LOG << e.what() << std::endl;
            }

            catch (...) {
                NAR_LOG<<"pull.cpp inside"<<std::endl;
                nar::MessageTypes::InfoChunkPull::Request _req(elements[i].chunk_id, 704);
                nar::MessageTypes::InfoChunkPull::Response _resp;
                _req.send_mess(server_sck,_resp);
                elements[i].stream_id = _resp.get_stream_id();
                if (buf)
                    delete [] buf;
                continue;
            }


            cli_sck->close();
            i++;
        }
    }
    catch ( ... ) {
        NAR_LOG << "Problem occured when pulling chunks from peers" << std::endl;
        delete tempfile1;
        return;
    }

    delete tempfile1;
    tempfile1 = new nar::File(temp_native,"r",true);
    nar::File * decrypted;
    try{
        decrypted = tempfile1->decrypt(file_aes);
    } catch(...) {
        NAR_LOG<<"can not decrytp"<<std::endl;
    }

    string dust= tpath.native();
    nar::File * decompressed = decrypted->decompress(dust);


    nar::MessageTypes::InfoChunkPull::Request _req(50, 200);
    _req.send_mess(server_sck);
    try {
        nar::MessageTypes::IPCPull::Response ipcpull_resp(100,200);

        ipcpull_resp.send_message_end(ipc_socket);
    } catch (...){
        NAR_LOG << "cli seems down!!! " << std::endl;
    }
    delete tempfile1;
    delete decrypted;
    delete decompressed;
    return;
}
