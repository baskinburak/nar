#ifndef NAR_FILEKEEPER_H
#define NAR_FILEKEEPER_H


#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <fcntl.h>

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>

#include <fstream>

namespace nar {
	class FileKeeper {
		private:
			char * _file;
			int _fd;
			std::map<std::string, int> _fds;

		public:
			FileKeeper() {
				_file = NULL;
				_fd = -1;
			}
			FileKeeper(char * file);
			FileKeeper(std::string file);

			~FileKeeper();

			void closeFd(int fd);					// closes the given file descriptor
			void emptyMap();						// empties the mapping with closing file descriptors

			int openFdRdonly(const char * file);		// opens file descriptor on read only mode
			static int openFdWrtonly(const char * file);
			static int openFdWrtonlyAppend(const char * file);
            int openAllWays(const char * file);

			void setFileName(char * file);			// to change file name which filekeeper holds
			void setFileName(std::string file);
			std::string getFileName();

				// creates file descriptor of the given file

			int getBytes(size_t start, size_t buffersize, char * buffer);			// gets specific byte interval  from the file

			static int writeToFile(int fd, size_t buffersize, const char * buffer);

			void setFileMap(std::vector< std::string > & filenames);				// creates a mapping between file names and their file descriptors (if there is an already existing mapping changes it to new one)
			void insertOneToMap(std::string filename);								// inserts one entry to file to descriptor mapping
			void removeOneFromMap(std::string filename);								// removes from file to descriptor mapping
			int getBytesMap(std::string file,size_t start, size_t buffersize, char * buffer); //gets specific byte interval from a file that is in the mapping
			void printMap();
			unsigned long getFileSize();
            int getFd();
	};
}
#endif //NAR_FILEKEEPER_H
