#include <nar/narnode/FileKeeper/FileCompressor.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <nar/narnode/utility.h>

unsigned long nar::FileCompressor::file_size(char *filename)
{
	FILE *pFile = fopen(filename, "rb");
	fseek (pFile, 0, SEEK_END);
	unsigned long size = ftell(pFile);
	fclose (pFile);
	return size;
}

int nar::FileCompressor::compress_one_file(nar::FileKeeper *myinfile, nar::FileKeeper *myoutfile)
{
    char *infilename = new char[myinfile->getFileName().length() + 1];
    strcpy(infilename, myinfile->getFileName().c_str());


    char *outfilename = new char[myoutfile->getFileName().length() + 1];
    strcpy(outfilename, myoutfile->getFileName().c_str());


    FILE *infile = fopen(infilename, "rb");
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
       total_read += num_read;
       gzwrite(outfile, inbuffer, num_read);
    }
    fclose(infile);
    gzclose(outfile);

    printf("Read %ld bytes, Wrote %ld bytes,Compression factor %4.2f%%\n",
       total_read, file_size(outfilename),
       (1.0-file_size(outfilename)*1.0/total_read)*100.0);
    delete [] infilename;
    delete [] outfilename;
}

int nar::FileCompressor::decompress_one_file(nar::FileKeeper *myinfile, nar::FileKeeper *myoutfile)
{
    char *infilename = new char[myinfile->getFileName().length() + 1];
    strcpy(infilename, myinfile->getFileName().c_str());


    char *outfilename = new char[myoutfile->getFileName().length() + 1];
    strcpy(outfilename, myoutfile->getFileName().c_str());

	gzFile infile = gzopen(infilename, "rb");
   	FILE *outfile = fopen(outfilename, "wb");
   	if (!infile || !outfile) return -1;
    char buffer[128];
    int num_read = 0;
    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
       fwrite(buffer, 1, num_read, outfile);
    }
    gzclose(infile);
    fclose(outfile);

    delete [] infilename;
    delete [] outfilename;
}

/*
compress_one_file(file1,file2);
decompress_one_file(file1,file2);
*/
